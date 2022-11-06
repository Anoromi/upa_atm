//
// Created by Andrii on 10/22/2022.
//

#include "signedConnection.h"


TransferDetails SignedConnection::getTransferDetails(const TransferRequest &r) {
    return bank.authorizedCall(Authorized(credentials(), r), Bank::getTransferDetails);
}

void SignedConnection::transferMoney(const TransferRequest & r) {
    return bank.authorizedCall(Authorized(credentials(), r), Bank::transferMoney);
}

DepositDetails SignedConnection::getDepositDetails(const DepositRequest & r) {
    return bank.authorizedCall(Authorized(credentials(), r), Bank::getDepositDetails);
}

void SignedConnection::depositMoney(const DepositRequest & r ) {
    return bank.authorizedCall(Authorized(credentials(), r), Bank::depositMoney);
}

WithdrawalDetails SignedConnection::getWithdrawalDetails(const WithdrawalRequest & r) {
    return bank.authorizedCall(Authorized(credentials(), r), Bank::getWithdrawalDetails);
}

void SignedConnection::withdrawMoney(const WithdrawalRequest & r) {
    return bank.authorizedCall(Authorized(credentials(), r), Bank::withdrawMoney);
}

SignedConnection::SignedConnection(const ConnectionDetails &details) : _details(details) {}
