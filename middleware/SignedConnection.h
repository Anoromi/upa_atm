//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_SIGNEDCONNECTION_H
#define BOOBLEEQUE_ATM_SIGNEDCONNECTION_H

#include "transfer.h"
#include "deposit.h"

class SignedConnection {
    Credentials _credentials;
public:
    TransferDetails getTransferDetails(const TransferRequest &);

    void transferMoney(const TransferRequest &);

    DepositDetails getDepositDetails(const DepositRequest &);

    void depositMoney(const DepositRequest &);

    WithdrawalDetails getWithdrawalDetails(const WithdrawalRequest &);

    void withdrawMoney(const WithdrawalRequest &);


};

//class JuniorConnection : public SignedConnection {
//};
#endif //BOOBLEEQUE_ATM_SIGNEDCONNECTION_H
