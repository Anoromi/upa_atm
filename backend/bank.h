//
// Created by Andrii on 10/30/2022.
//

#ifndef UPA_ATM_BANK_H
#define UPA_ATM_BANK_H

#include <QSqlDatabase>
#include "middleware/deposit.h"
#include "middleware/transfer.h"
#include "middleware/exceptions.h"
#include "middleware/card_info.h"

class Bank {
private:
    class InternalBank {
    private:
        QSqlDatabase _db;
        Set<ullong> _blocked_cards;

        uint getSpendableMoney(const Credentials &c);

        uint cardBalance(const Card &c);

        void addMoney(const Card &c, uint change);

        void removeMoney(const Card &c, uint change);


        void addTransaction(std::optional<Card> sender,
                            std::optional<Card> receiver,
                            uint amount, uint fee);

        bool isBlocked(const Credentials &c) {
            return _blocked_cards.find(c.card().getCardNumber()) != _blocked_cards.end();
        }

    public:
        InternalBank() : _db(QSqlDatabase::database()) {}

        /*
         * Returns true if
         * 1. card with given number exists
         * 2. pin is correct
         * 3. card is not expired
         * 4. card is not blocked
         *
         * Blocks card if it is expired.
         */
        bool areValidCredentials(const Credentials &c);

        void blockCard(const Card &card);
        CardInfo getCardInfo(const Credentials &с);

        void transferMoney(const Credentials &c, const TransferRequest &request);
        void depositMoney(const Credentials &c, const DepositRequest &request);
        void withdrawMoney(const Credentials &с, const WithdrawalRequest &request);

        TransferDetails getTransferDetails(const Credentials &c, const TransferRequest &request);
        DepositDetails getDepositDetails(const Credentials &c, const DepositRequest &request);
        WithdrawalDetails getWithdrawalDetails(const Credentials &c, const WithdrawalRequest &request);

        /*
         * If parent relation is not exist
         */
        void limitChildMoney(const Credentials &с, const Card &card, const uint &money);
    };

    InternalBank _internalBank;
public:
    template<typename R, typename... Ad>
    using Request = R (InternalBank::*)(const Credentials &, const Ad &...);

    template<typename R, typename... Ad>
    R authorizedCall(const Credentials &cr, Request<R, Ad...> request, const Ad &... ar) {
        if (!_internalBank.areValidCredentials(cr)) {
            throw UnexpectedException(L"Credentials aren't valid");
        }
        return (_internalBank.*request)(cr, ar...);
    }

    inline bool areValidCredentials(const Credentials &c) {
        return _internalBank.areValidCredentials(c);
    }

    inline void blockCard(const Card &card) {
        return _internalBank.blockCard(card);
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
    constexpr static Bank::Request<CardInfo> getCardInfo = &InternalBank::getCardInfo;
};

#endif //UPA_ATM_BANK_H
