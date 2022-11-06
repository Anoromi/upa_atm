//
// Created by Andrii on 11/6/2022.
//

#ifndef UPA_ATM_SIGNEDCONNECTION_H
#define UPA_ATM_SIGNEDCONNECTION_H
#include "transfer.h"
#include "deposit.h"
#include "backend/bankProvider.h"

class SignedConnection {
    Credentials _credentials;
    Bank& bank = BankProvider::getBank();
public:
    explicit SignedConnection(const Credentials &credentials);

    TransferDetails getTransferDetails(const TransferRequest &);

    void transferMoney(const TransferRequest &);


    void withdrawMoney(const WithdrawalRequest &);

    DepositDetails getDepositDetails(const DepositRequest &r);

    void depositMoney(const DepositRequest &r);

    WithdrawalDetails getWithdrawalDetails(const WithdrawalRequest &r);
};

#endif //UPA_ATM_SIGNEDCONNECTION_H
