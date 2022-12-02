#include "bank.h"

WithdrawalDetails Bank::InternalBank::getWithdrawalDetails(const Credentials &, const WithdrawalRequest &) {
    throw UnexpectedException(L"not implemented");
}

void Bank::InternalBank::withdrawMoney(const Credentials &, const WithdrawalRequest &) {
    throw UnexpectedException(L"not implemented");
}

void Bank::InternalBank::limitChildMoney(const Credentials &, const Card &card, const uint &money) {
    throw UnexpectedException(L"not implemented");
}
