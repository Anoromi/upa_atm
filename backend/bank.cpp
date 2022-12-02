#include "bank.h"
#include "backend/database/db_holder.h"
#include "middleware/exceptions.h"
#include "middleware/types.h"
#include "backend/database/db_card.h"
#include "backend/database/db_transaction.h"
#include "backend/database/db_category.h"
#include "qsqlquery.h"

llong Bank::InternalBank::getSpendableMoney(const Credentials &c) {
    throw UnexpectedException(L"Unimplemented");
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
    const ullong rNumber = receiver.getNumber().value();
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

void Bank::InternalBank::depositMoney(const Credentials &c, const DepositRequest &request)
{
    DepositDetails details = getDepositDetails(c, request);
    _db.transaction();
    addTransaction(c.card(), std::optional<Card>(), details.getMoney(),
                   details.getTariff().getFee(details.getMoney()));
    _db.transaction();
}

WithdrawalDetails Bank::InternalBank::getWithdrawalDetails(const Credentials &, const WithdrawalRequest &) {
    throw UnexpectedException(L"not implemented");
}

void Bank::InternalBank::withdrawMoney(const Credentials &, const WithdrawalRequest &) {
    throw UnexpectedException(L"not implemented");
}

void Bank::InternalBank::limitChildMoney(const Credentials &, const Card &card, const uint &money) {
    throw UnexpectedException(L"not implemented");
}
