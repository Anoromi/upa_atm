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

        bool areValidCredentials(Credentials);

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
    using Request = R (InternalBank::*)(T);

    template<typename T, typename R>
    inline R authorizedCall(Authorized<T> a, Request<T, R> request) {
        if(!_internalBank.areValidCredentials(a.credentials())) {
            throw UnexpectedException("Credentials aren't valid");
        }
        _internalBank.*request(a.value());
    }

private:

public:
    const Bank::Request<const TransferRequest &, TransferDetails> getTransferDetails = &InternalBank::getTransferDetails;
    const Bank::Request<const TransferRequest &> transferMoney = &InternalBank::transferMoney;
    const Bank::Request<const DepositRequest &, DepositDetails> getDepositDetails = &InternalBank::getDepositDetails;
    const Bank::Request<const DepositRequest &> depositMoney = &InternalBank::depositMoney;
    const Bank::Request<const WithdrawalRequest &, WithdrawalDetails> getWithdrawalDetails = &InternalBank::getWithdrawalDetails;
    const Bank::Request<const WithdrawalRequest &> withdrawMoney = &InternalBank::withdrawMoney;
};


#endif //UPA_ATM_BANK_H
