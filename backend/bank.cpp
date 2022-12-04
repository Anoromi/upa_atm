#include "bank.h"
#include "backend/database/db_holder.h"
#include "middleware/types.h"
#include "backend/database/db_card.h"
#include "backend/database/db_transaction.h"
#include "backend/database/db_category.h"
#include "backend/database/db_parent_relation.h"
#include "middleware/exceptions.h"
#include <QSqlError>

uint Bank::InternalBank::getSpendableMoney(const Card &c) {
    ullong cardNumber = c.getCardNumber();
    Optional<DBParentRelation> rel = DBParentRelation::selectByChildId(cardNumber, _db);
    ullong balance = cardBalance(cardNumber);
    if (!rel.has_value()) { // it is not a child card
        return balance;
    }
    ullong dayLimit = rel.value().getDayLimit().value();
    Vector<DBTransaction> todayTransactions =
            DBTransaction::selectSpendingsByPeriod(cardNumber,
                                                   QDate::currentDate().startOfDay(),
                                                   QDateTime::currentDateTime(),
                                                   _db);
    ullong todaySpendings = 0;
    for (auto &t : todayTransactions) {
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
    uint spendable = getSpendableMoney(c);
    if (spendable < change) {
        throw BadMoney(change, spendable);
    }
    DBCard card;
    card.setNumber(c.getCardNumber());
    card.setBalance(balance - change);
    DBCard::update(card, _db);
}

/*
 * amount will be charged from sender
 * amount - fee will be send to receiver
 */
void Bank::InternalBank::addTransaction(std::optional<Card> sender,
                                        std::optional<Card> receiver,
                                        uint amount, uint fee) {
    _db.transaction();
    try {
        if (sender.has_value()) {
            removeMoney(sender.value(), amount);
        }
        if (receiver.has_value()) {
            addMoney(receiver.value(), amount - fee);
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
            if (!_db.rollback()) {
                qDebug() << "Could not rollback";
            }
            throw UnexpectedException(_db.lastError().databaseText().toStdWString());
        }
    } catch (...) {
        _db.rollback();
        throw;
    }
}

bool Bank::InternalBank::areValidCredentials(const Credentials &c) {
    if (isBlocked(c)) {
        throw BlockedCard();
    }
    try {
        Card card = c.card();
        DBCard cardInfo = DBCard::selectByNumber(card.getCardNumber());
        if (QDate::currentDate() > cardInfo.getExpirationDate().value()) {
            blockCard(card);
            return false;
        }
        return cardInfo.getPin().value() == c.pin().getPin();
    } catch (...) {
        return false;
    }
}

TransferDetails Bank::InternalBank::getTransferDetails(const Credentials &c, const TransferRequest &request) {
    DBCard sender = DBCard::selectByNumber(c.card().getCardNumber());
    DBCard receiver;
    try {
        receiver = DBCard::selectByNumber(request.getDestination().getCardNumber());
    } catch (...) {
        throw BadRecipient(request.getDestination());
    }
    const ullong rHolder = receiver.getHolderId().value();
    DBHolder receiverInfo = DBHolder::selectById(rHolder);;
    DBCategory category = DBCategory::selectById(sender.getCategoryId().value());
    auto tariff = Unique<Tariff>(new PercentageTariff(category.getFeeRate().value()));
    uint actual = request.getMoney();
    if (request.isAfterTariff()) {
        actual += tariff->getFee(request.getMoney());
    }
    return {receiverInfo.getFullName().toStdWString(),
            request.getDestination(),
            actual,
            std::move(tariff)};
}

void Bank::InternalBank::transferMoney(const Credentials &from, const TransferRequest &request) {
    TransferDetails details = getTransferDetails(from, request);
    uint moneyToRemove = details.getMoney();
    uint fee = details.getTariff().getFee(request.getMoney());
    addTransaction(from.card(),
                   request.getDestination().getCardNumber(),
                   moneyToRemove,
                   request.isAfterTariff() ? fee : 0);
}

DepositDetails Bank::InternalBank::getDepositDetails(const Credentials &c, const DepositRequest &request)
{
    uint previousBalance = cardBalance(c.card());
    auto tariff = Unique<Tariff>(new WholeTariff(10)); // todo where to get this number?
    uint fee = tariff->getFee(request.getMoney());
    return {request.getMoney() - fee, std::move(tariff), previousBalance};
}

void Bank::InternalBank::depositMoney(const Credentials &to, const DepositRequest &request) {
    DepositDetails details = getDepositDetails(to, request);
    uint fee = details.getTariff().getFee(request.getMoney());
    uint moneyToAdd = request.getMoney();
    addTransaction(Optional<Card>(), to.card(), moneyToAdd, fee);
}

WithdrawalDetails Bank::InternalBank::getWithdrawalDetails(const Credentials &c, const WithdrawalRequest &request) {
    DBCard sender = DBCard::selectByNumber(c.card().getCardNumber());
    DBCategory category = DBCategory::selectById(sender.getCategoryId().value());
    auto tariff = std::make_unique<PercentageTariff>(category.getFeeRate().value());
    uint moneyToBeWithdrawed = request.getMoney();
    if (request.isAfterTariff()) {
        moneyToBeWithdrawed -= tariff->getFee(moneyToBeWithdrawed);
    }
    return {moneyToBeWithdrawed, std::move(tariff)};
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

CardInfo Bank::InternalBank::getCardInfo(const Credentials &c) {
    DBCard card = DBCard::selectByNumber(c.card().getCardNumber(), _db);
    DBHolder holder = DBHolder::selectById(card.getHolderId().value(), _db);
    return {holder.getFullName().toStdWString(), (uint) card.getBalance().value()};
}

void Bank::InternalBank::blockCard(const Card &card) {
    _blocked_cards.insert(card.getCardNumber());
}
