//
// Created by Andrii on 10/30/2022.
//

#ifndef UPA_ATM_BANK_H
#define UPA_ATM_BANK_H


#include "middleware/transfer.h"
#include "middleware/deposit.h"
#include "middleware/exceptions.h"

class Bank {
private:
    class InternalBank {

    public:

        bool areValidCredentials(const Credentials &);

        TransferDetails getTransferDetails(const Credentials &, const TransferRequest &);

        void transferMoney(const Credentials &, const TransferRequest &);

        DepositDetails getDepositDetails(const Credentials &, const DepositRequest &);

        void depositMoney(const Credentials &, const DepositRequest &);

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
