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
    uint incorrectVerifications;
public:
    bool validateCredentials(const Credentials &c) {
        return bank.areValidCredentials(c);
    }

    Shared<SignedConnection> createConnection(const Credentials &c) {
        if (incorrectVerifications++ >= 3) {
            bank.blockCard(c.card());
        }
        return Shared<SignedConnection>(new SignedConnection(ConnectionDetails(c)));
    }
};

#endif //UPA_ATM_UNSIGNEDCONNECTION_H
