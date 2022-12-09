//
// Created by Andrii on 10/22/2022.
//

#include "signedConnection.h"


TransferDetails SignedConnection::getTransferDetails(const TransferRequest &r) {
    return bank.authorizedCall(credentials(), Bank::getTransferDetails, r);
}

void SignedConnection::transferMoney(const TransferRequest &r) {
    return bank.authorizedCall(credentials(), Bank::transferMoney, r);
}

DepositDetails SignedConnection::getDepositDetails(const DepositRequest &r) {
    return bank.authorizedCall(credentials(), Bank::getDepositDetails, r);
}

void SignedConnection::depositMoney(const DepositRequest &r) {
    return bank.authorizedCall(credentials(), Bank::depositMoney, r);
}

WithdrawalDetails SignedConnection::getWithdrawalDetails(const WithdrawalRequest &r) {
    return std::move(bank.authorizedCall(credentials(), Bank::getWithdrawalDetails, r));
}

void SignedConnection::withdrawMoney(const WithdrawalRequest &r) {
    return bank.authorizedCall(credentials(), Bank::withdrawMoney, r);
}

SignedConnection::SignedConnection(const ConnectionDetails &details) : _details(details) {}

Vector<Transaction> SignedConnection::getTransactions() {
    return std::move(bank.authorizedCall(credentials(), Bank::getTransactions));
}

void SignedConnection::topUpMoney(const TopUpRequest &r) {
    bank.authorizedCall(credentials(), Bank::performTopUp, r);
}

void ParentConnection::limitChildMoney(uint money) {
    bank.authorizedCall(credentials(), Bank::limitChildMoney, _childCard.getCard(), money);
}

