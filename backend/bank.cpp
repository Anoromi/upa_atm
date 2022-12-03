#include "bank.h"
#include "backend/database/db_holder.h"
#include "middleware/types.h"
#include "backend/database/db_card.h"
#include "backend/database/db_transaction.h"
#include "backend/database/db_category.h"
#include "backend/database/db_parent_relation.h"
#include "qsqlquery.h"
#include "middleware/card_info.h"

uint Bank::InternalBank::getSpendableMoney(const Credentials &c) {
    Optional<DBParentRelation> rel = DBParentRelation::selectByChildId(c.card().getCardNumber(), _db);
    ullong balance = cardBalance(c.card());
    if (!rel.has_value()) { // it is not a child card
        return balance;
    }
    ullong dayLimit = rel.value().getDayLimit().value();
    Vector<DBTransaction> todayTransactions =
            DBTransaction::selectSpendingsByPeriod(c.card().getCardNumber(),
                                                   QDate::currentDate().startOfDay(),
                                                   QDateTime::currentDateTime(),
                                                   _db);
    ullong todaySpendings = 0;
    for (auto &t: todayTransactions) {
        todaySpendings += t.getAmount().value();
    }
    return dayLimit - todaySpendings;
}

uint Bank::InternalBank::cardBalance(const Card &c) {
    DBCard card = DBCard::selectByNumber(c.getCardNumber(), _db);
    return card.getBalance().value();
}

void Bank::InternalBank::addMoney(const Card &c, uint change) {
    DBCard card;
    card.setNumber(c.getCardNumber());
    card.setBalance(cardBalance(c) + change);
    DBCard::update(card, _db);
}

void Bank::InternalBank::removeMoney(const Card &c, uint change) {
    uint balance = cardBalance(c);
    if (balance < change) {
        throw UnexpectedException(L"Trying to remove too much money");
    }
    DBCard card;
    card.setNumber(c.getCardNumber());
    card.setBalance(balance - change);
    DBCard::update(card, _db);
}

void Bank::InternalBank::addTransaction(std::optional<Card> sender,
                                        std::optional<Card> receiver,
                                        uint amount, uint fee) {
    if (sender.has_value()) {
        removeMoney(sender.value(), amount);
    }
    if (receiver.has_value()) {
        addMoney(receiver.value(), amount - fee);
    }

    DBTransaction bankTransaction;
    if (sender.has_value())
        bankTransaction.setSenderId(sender.value().getCardNumber());
    if (receiver.has_value())
        bankTransaction.setReceiverId(receiver.value().getCardNumber());
    bankTransaction.setAmount(amount);
    bankTransaction.setFee(fee);
    bankTransaction.setTime(QDateTime::currentDateTime());
}

bool Bank::InternalBank::areValidCredentials(const Credentials &c) {
    try {
        DBCard card = DBCard::selectByNumber(c.card().getCardNumber());
        return card.getPin().value() == c.pin().getPin();
    } catch (...) {
        return false;
    }
}

TransferDetails Bank::InternalBank::getTransferDetails(const Credentials &c, const TransferRequest &request) {
    DBCard receiver;
    try {
        receiver = DBCard::selectByNumber(c.card().getCardNumber());
    } catch (...) {
        throw BadRecipient(request.getDestination());
    }
    const ullong rHolder = receiver.getHolderId().value();
    DBHolder receiverInfo;
    try {
        receiverInfo = DBHolder::selectById(rHolder);
    } catch (...) {
        throw UnexpectedException(L"Holder with id wasn't found");
    }

    String holderName = receiverInfo.getSurname().value().toStdWString() + L" " +
                        receiverInfo.getName().value().toStdWString();

    DBCategory category;
    try {
        category = DBCategory::selectById(receiver.getCategoryId().value());
    } catch (...) {
        throw;
    }
    auto tariff = std::make_unique<PercentageTariff>(category.getFeeRate().value());
    uint actualInitial;
    if (request.isAfterTariff()) {
        actualInitial = tariff->getInitial(request.getMoney());
    } else {
        actualInitial = request.getMoney();
    }
    uint spendable = getSpendableMoney(c);
    if (actualInitial > spendable) {
        throw BadMoney(actualInitial, spendable);
    }
    return {holderName, request.getDestination(), actualInitial, std::move(tariff)};
}

void Bank::InternalBank::transferMoney(const Credentials &c, const TransferRequest &request) {
    TransferDetails details = getTransferDetails(c, request);
    _db.transaction();
    try {
        removeMoney(c.card(), details.getMoney());
        addTransaction(c.card(), request.getDestination().getCardNumber(), details.getMoney(),
                       details.getTariff().getFee(details.getMoney()));
        addMoney(request.getDestination(), details.getMoney());
        if (!_db.commit()) {
            _db.rollback();
            throw UnexpectedException(L"Cound not commit to db");
        }
    } catch (...) {
        _db.rollback();
        throw;
    }
}

DepositDetails Bank::InternalBank::getDepositDetails(const Credentials &c, const DepositRequest &request) {
    uint previousBalance = cardBalance(c.card());
    return {request.getMoney(), Unique<Tariff>(new WholeTariff(10)), previousBalance};
}

void Bank::InternalBank::depositMoney(const Credentials &c, const DepositRequest &request) {
    DepositDetails details = getDepositDetails(c, request);
    _db.transaction();
    try {
        addMoney(c.card().getCardNumber(), details.getMoney());
        addTransaction(Optional<Card>(), c.card(), details.getMoney(),
                       details.getTariff().getFee(details.getMoney()));
        if (!_db.commit()) {
            _db.rollback();
            throw UnexpectedException(L"Cound not commit to db");
        }
    } catch (...) {
        _db.rollback();
        throw;
    }
}

WithdrawalDetails Bank::InternalBank::getWithdrawalDetails(const Credentials &c, const WithdrawalRequest &request) {
    DBCard sender;
    try {
        sender = DBCard::selectByNumber(c.card().getCardNumber());
    } catch (...) {
        throw UnexpectedException(L"Cannot withdraw: unknown account");
    }
    const ullong sHolder = sender.getHolderId().value();
    DBHolder senderInfo;
    try {
        senderInfo = DBHolder::selectById(sHolder);
    } catch (...) {
        throw UnexpectedException(L"Holder with id wasn't found");
    }

    String holderName = senderInfo.getSurname().value().toStdWString() + L" " +
                        senderInfo.getName().value().toStdWString();

    DBCategory category;
    try {
        category = DBCategory::selectById(sender.getCategoryId().value());
    } catch (...) {
        throw;
    }
    auto tariff = std::make_unique<PercentageTariff>(category.getFeeRate().value());
    uint actualInitial;
    if (request.isAfterTariff()) {
        actualInitial = tariff->getInitial(request.getMoney());
    } else {
        actualInitial = request.getMoney();
    }
    uint spendable = getSpendableMoney(c);
    if (actualInitial > spendable) {
        throw BadMoney(actualInitial, spendable);
    }
    return {actualInitial, std::move(tariff)};
}

void Bank::InternalBank::withdrawMoney(const Credentials &c, const WithdrawalRequest &request) {
    WithdrawalDetails details = getWithdrawalDetails(c, request);
    _db.transaction();
    try {
        removeMoney(c.card(), details.getMoney());
        addTransaction(c.card(), Optional<Card>(),
                       details.getMoney(),
                       details.getTariff()->getFee(request.getMoney()));
        if (!_db.commit()) {
            _db.rollback();
            throw UnexpectedException(L"Cound not commit to db");
        }
    } catch (...) {
        _db.rollback();
        throw;
    }
}

void Bank::InternalBank::limitChildMoney(const Credentials &parentCred, const Card &childCard, const uint &money) {
    Optional<DBParentRelation> rel = DBParentRelation::selectByChildId(childCard.getCardNumber(), _db);
    if (rel.has_value()) {
        DBParentRelation relInfo = rel.value();
        if (relInfo.getParentCardId() == parentCred.card().getCardNumber()) {
            if (money != relInfo.getDayLimit().value()) {
                relInfo.setDayLimit(money);
                DBParentRelation::update(relInfo, _db);
            }
        } else {
            throw UnexpectedException(L"Cannot limit: this card was limited by other user");
        }
    } else {
        DBParentRelation relInfo(parentCred.card().getCardNumber(),
                                 childCard.getCardNumber(),
                                 money);
        DBParentRelation::create(relInfo, _db);
    }
}

CardInfo Bank::InternalBank::getCardInfo(const Credentials &c) {
}

void Bank::InternalBank::blockCard(const Card &card) {

}
