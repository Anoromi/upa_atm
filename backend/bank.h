//
// Created by Andrii on 10/30/2022.
//

#ifndef UPA_ATM_BANK_H
#define UPA_ATM_BANK_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "middleware/transfer.h"
#include "middleware/deposit.h"
#include "middleware/exceptions.h"

class Bank {
private:
    class InternalBank {
    private:
        QSqlDatabase _db;

        llong getExpendableMoney(const Credentials &c) {
            throw UnexpectedException("Unimplemented");
        }

        uint cardBalance(const Card &c) {
            QSqlQuery query;
            query.prepare("SELECT balance FROM Card WHERE number = ?");
            query.addBindValue(c.getCardNumber());
        }

        void addMoney(const Card &c, uint change) {
            uint balance = cardBalance(c);
            QSqlQuery query(_db);
            query.prepare("UPDATE Card SET balance = ? WHERE number = ?");
            query.addBindValue(balance + change);
            query.addBindValue(c.getCardNumber());
            if (!query.exec()) {
                throw UnexpectedException("Error adding money to card");
            }
        }

        void removeMoney(const Card &c, uint change) {
            uint balance = cardBalance(c);
            if (balance < change) {
                throw UnexpectedException("Trying to remove too much money");
            }
            QSqlQuery query(_db);
            query.prepare("UPDATE Card SET balance = ? WHERE number = ?");
            query.addBindValue(balance - change);
            query.addBindValue(c.getCardNumber());
            if (!query.exec()) {
                throw UnexpectedException("Error adding money to card");
            }
        }

    public:
        InternalBank() : _db(QSqlDatabase::addDatabase("QSQLITE")) {
            _db.setHostName("upa");
            _db.open();
        }

        bool areValidCredentials(const Credentials &c) {
            QSqlQuery query(_db);
            query.prepare("SELECT 1 FROM Card WHERE number = ? AND pin = ?");
            query.addBindValue(c.card().getCardNumber());
            query.addBindValue(c.pin().getPin());
            query.exec();
            return query.next();
        }


        TransferDetails getTransferDetails(const Credentials &c, const TransferRequest &request) {
            QSqlQuery getReceiver(_db);
            getReceiver.prepare("SELECT (number,holder_id) FROM Card WHERE number = ?");
            getReceiver.addBindValue(request.getDestination().getCardNumber());
            getReceiver.exec();
            if (!getReceiver.next()) {
                throw BadRecipient(request.getDestination());
            }
            const ullong rNumber = getReceiver.value(0).toULongLong();
            const ullong rHolder = getReceiver.value(1).toULongLong();
            QSqlQuery getHolderName(_db);
            getHolderName.prepare("SELECT (name, surname) FROM Holder WHERE id = ?");
            getHolderName.addBindValue(rHolder);
            getHolderName.exec();
            if (!getHolderName.next()) {
                throw UnexpectedException("Holder with id wasn't found");
            }
            String holderName = (getHolderName.value(0).toString() + getHolderName.value(1).toString()).toStdString();

            PercentageTariff *tariff = new PercentageTariff(0.5);
            uint actualInitial;
//            uint actualTariffed;
            if (request.isAfterTariff()) {
                actualInitial = tariff->getInitial(request.getMoney());
//                actualTariffed = request.getMoney();
            } else {
                actualInitial = request.getMoney();
//                actualTariffed = tariff->getLeftOver(request.getMoney());
            }
            uint expendable = getExpendableMoney(c);
            if (actualInitial > expendable) {
                throw BadMoney(actualInitial, expendable);
            }

            return {holderName, request.getDestination(), actualInitial,
                    Unique<Tariff>(tariff)};

        }


        void transferMoney(const Credentials &c, const TransferRequest &request) {
            TransferDetails details = getTransferDetails(c, request);
            _db.transaction();
            removeMoney(c.card(), details.getMoney());
            addMoney(request.getDestination(), details.getMoney());
            _db.commit();
        }

        DepositDetails getDepositDetails(const Credentials & c, const DepositRequest & request) {
            QSqlQuery query(_db);
            query.prepare("SELECT balance FROM Card WHERE number = ?");
            query.addBindValue(c.card().getCardNumber());
            query.exec();
            query.next();
            uint previousBalance = query.value(0).toUInt();
            return {request.getMoney(), Unique<Tariff>(new WholeTariff(10)), previousBalance};
        }

        void depositMoney(const Credentials & c, const DepositRequest & request) {
            DepositDetails details = getDepositDetails(c, request);
            _db.transaction();
            addMoney(c.card(), request.getMoney());
            _db.transaction();
        }

        WithdrawalDetails getWithdrawalDetails(const Credentials &, const WithdrawalRequest &);

        void withdrawMoney(const Credentials &, const WithdrawalRequest &);

        void limitChildMoney(const Credentials &, const Card &card, const uint &money);

    };

    InternalBank _internalBank;
public:
    template<typename R, typename... Ad>
    using Request = R (InternalBank::*)(const Credentials &, const Ad &...);

    template<typename R, typename... Ad>
    R authorizedCall(const Credentials &cr, Request<R, Ad...> request, const Ad &... ar) {
        if (!_internalBank.areValidCredentials(cr)) {
            throw UnexpectedException("Credentials aren't valid");
        }
        return (_internalBank.*request)(cr, ar...);
    }

    inline bool areValidCredentials(const Credentials &c) {
        return _internalBank.areValidCredentials(c);
    }


private:

public:
    constexpr static Bank::Request<TransferDetails, TransferRequest> getTransferDetails = &InternalBank::getTransferDetails;
    constexpr static Bank::Request<void, TransferRequest> transferMoney = &InternalBank::transferMoney;
    constexpr static Bank::Request<DepositDetails, DepositRequest> getDepositDetails = &InternalBank::getDepositDetails;
    constexpr static Bank::Request<void, DepositRequest> depositMoney = &InternalBank::depositMoney;
    constexpr static Bank::Request<WithdrawalDetails, WithdrawalRequest> getWithdrawalDetails = &InternalBank::getWithdrawalDetails;
    constexpr static Bank::Request<void, WithdrawalRequest> withdrawMoney = &InternalBank::withdrawMoney;
    constexpr static Bank::Request<void, Card, uint> limitChildMoney = &InternalBank::limitChildMoney;
};


#endif //UPA_ATM_BANK_H
