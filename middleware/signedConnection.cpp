//
// Created by Andrii on 10/22/2022.
//

#include "signedConnection.h"

SignedConnection::SignedConnection(const Credentials &credentials) : _credentials(credentials) {}

TransferDetails SignedConnection::getTransferDetails(const TransferRequest &r) {
    return bank.authorizedCall(Authorized(_credentials, r), Bank::getTransferDetails);
}

void SignedConnection::transferMoney(const TransferRequest & r) {
    return bank.authorizedCall(Authorized(_credentials, r), Bank::transferMoney);
}

DepositDetails SignedConnection::getDepositDetails(const DepositRequest & r) {
    return bank.authorizedCall(Authorized(_credentials, r), Bank::getDepositDetails);
}

void SignedConnection::depositMoney(const DepositRequest & r ) {
    return bank.authorizedCall(Authorized(_credentials, r), Bank::depositMoney);
}

WithdrawalDetails SignedConnection::getWithdrawalDetails(const WithdrawalRequest & r) {
    return bank.authorizedCall(Authorized(_credentials, r), Bank::getWithdrawalDetails);
}

void SignedConnection::withdrawMoney(const WithdrawalRequest & r) {
    return bank.authorizedCall(Authorized(_credentials, r), Bank::withdrawMoney);
}
// Created by Andrii on 11/6/2022.
//

#include "signedConnection.h"
