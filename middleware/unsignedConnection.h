//
// Created by Andrii on 04.11.2022.
//

#ifndef UPA_ATM_UNSIGNEDCONNECTION_H
#define UPA_ATM_UNSIGNEDCONNECTION_H

#include "backend/bankProvider.h"
#include "signedConnection.h"

class UnsignedConnection {
private:
    Bank &bank = BankProvider::getBank();
public:
    bool validateCredentials(const Credentials &c) {
        return bank.areValidCredentials(c);
    }

    Shared<SignedConnection> createConnection(const Credentials &c) {
        if (bank.areValidCredentials(c)) {
            return Shared<SignedConnection>(new SignedConnection({c, true}));
        }
        throw UnexpectedException(L"Invalid data");
    }
};

#endif //UPA_ATM_UNSIGNEDCONNECTION_H
