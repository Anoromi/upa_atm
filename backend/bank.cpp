#include "bank.h"
#include "backend/database/db_holder.h"
#include "middleware/types.h"
#include "backend/database/db_card.h"
#include "backend/database/db_transaction.h"
#include "backend/database/db_category.h"
#include "backend/database/db_parent_relation.h"
#include "qsqlquery.h"

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
    for (auto& t : todayTransactions) {
        todaySpendings += t.getAmount().value();
    }
    return dayLimit < todaySpendings ? 0 : dayLimit - todaySpendings;
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
        // todo replace with explicit exception
        throw UnexpectedException(L"Trying to remove too much money");
    }
    DBCard card;
    card.setNumber(c.getCardNumber());
    card.setBalance(balance - change);
    DBCard::update(card, _db);
    int x = 5;
}

void Bank::InternalBank::addTransaction(std::optional<Card> sender,
                                        std::optional<Card> receiver,
                                        uint amount, uint fee) {
    _db.transaction();
        try {
            if (sender.has_value()) {
                removeMoney(sender.value(), amount + fee);
            }
            if (receiver.has_value()) {
                addMoney(receiver.value(), amount);
            }
            DBTransaction bankTransaction;
            if (sender.has_value()) {
                bankTransaction.setSenderId(sender.value().getCardNumber());
            }

            if (receiver.has_value()) {
                bankTransaction.setReceiverId(receiver.value().getCardNumber());
            }
            bankTransaction.setAmount(amount);
            bankTransaction.setFee(fee);
            bankTransaction.setTime(QDateTime::currentDateTime());
            DBTransaction::create(bankTransaction, _db);
            if (!_db.commit()) {
                _db.rollback();
                throw UnexpectedException(L"Cound not commit to db");
            }
        } catch (...) {
            _db.rollback();
            throw;
        }
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

    PercentageTariff *tariff = new PercentageTariff(category.getFeeRate().value());
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
    return {holderName, request.getDestination(), actualInitial, Unique<Tariff>(tariff)};
}

void Bank::InternalBank::transferMoney(const Credentials &from, const TransferRequest &request) {
    TransferDetails details = getTransferDetails(from, request);
        uint moneyToAdd = details.getMoney();
        uint fee = details.getTariff().getFee(moneyToAdd);
        addTransaction(from.card(),
                       request.getDestination().getCardNumber(),
                       moneyToAdd, fee);
}

DepositDetails Bank::InternalBank::getDepositDetails(const Credentials &c, const DepositRequest &request)
{
    QSqlQuery query(_db);
    query.prepare("SELECT balance FROM card WHERE number = ?");
    query.addBindValue(c.card().getCardNumber());
    query.exec();
    query.next();
    uint previousBalance = query.value(0).toUInt();
    return {request.getMoney(), Unique<Tariff>(new WholeTariff(10)), previousBalance};
}

void Bank::InternalBank::depositMoney(const Credentials &to, const DepositRequest &request) {
    DepositDetails details = getDepositDetails(to, request);
    uint moneyToAdd = details.getMoney();
    uint fee = details.getTariff().getFee(moneyToAdd);
    addTransaction(Optional<Card>(), to.card(), moneyToAdd, fee);
}

WithdrawalDetails Bank::InternalBank::getWithdrawalDetails(const Credentials &c, const WithdrawalRequest &request) {
    DBCard sender;
    try {
        sender = DBCard::selectByNumber(c.card().getCardNumber());
    } catch (...) {
        throw UnexpectedException(L"Cannot withdraw: unknown account");
    }

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
    addTransaction(c.card(), Optional<Card>(),
                       details.getMoney(),
                       details.getTariff()->getFee(request.getMoney()));
}

void Bank::InternalBank::limitChildMoney(const Credentials& parentCred, const Card& childCard, const uint& money) {
    Optional<DBParentRelation> rel = DBParentRelation::selectByChildId(childCard.getCardNumber(), _db);
    if (rel.has_value()) {
        DBParentRelation relInfo = rel.value();
        if (relInfo.getParentCardId() == parentCred.card().getCardNumber())
        {
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
