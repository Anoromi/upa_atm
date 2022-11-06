//
// Created by Andrii on 10/30/2022.
//

#ifndef UPA_ATM_BANK_H
#define UPA_ATM_BANK_H


#include "middleware/transfer.h"
#include "middleware/deposit.h"
#include "middleware/connectionExceptions.h"

class Bank {
private:
    class InternalBank {

    public:

        bool areValidCredentials(const Credentials&);

        TransferDetails getTransferDetails(const TransferRequest &);

        void transferMoney(const TransferRequest &);

        DepositDetails getDepositDetails(const DepositRequest &);

        void depositMoney(const DepositRequest &);

        WithdrawalDetails getWithdrawalDetails(const WithdrawalRequest &);

        void withdrawMoney(const WithdrawalRequest &);

    };

    InternalBank _internalBank;
public:
    template<typename T, typename R = void>
    using Request = R (InternalBank::*)(const T &);

    template<typename T, typename R>
    inline R authorizedCall(Authorized<T> a, Request<T, R> request) {
        if (!_internalBank.areValidCredentials(a.credentials())) {
            throw UnexpectedException("Credentials aren't valid");
        }
        return (_internalBank.*request)(a.value());
    }

    inline bool areValidCredentials(const Credentials& c) {
        return _internalBank.areValidCredentials(c);
    }
private:

public:
    constexpr static Bank::Request<TransferRequest, TransferDetails> getTransferDetails = &InternalBank::getTransferDetails;
    constexpr static Bank::Request<TransferRequest> transferMoney = &InternalBank::transferMoney;
    constexpr static Bank::Request<DepositRequest, DepositDetails> getDepositDetails = &InternalBank::getDepositDetails;
    constexpr static Bank::Request<DepositRequest> depositMoney = &InternalBank::depositMoney;
    constexpr static Bank::Request<WithdrawalRequest, WithdrawalDetails> getWithdrawalDetails = &InternalBank::getWithdrawalDetails;
    constexpr static Bank::Request<WithdrawalRequest> withdrawMoney = &InternalBank::withdrawMoney;
};


#endif //UPA_ATM_BANK_H
