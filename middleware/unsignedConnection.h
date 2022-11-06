//
// Created by Andrii on 04.11.2022.
//

#ifndef UPA_ATM_UNSIGNEDCONNECTION_H
#define UPA_ATM_UNSIGNEDCONNECTION_H

#include "backend/bankProvider.h"
#include "signedConnection.h"

class UnsignedConnection {
private:
    Bank& bank = BankProvider::getBank();
public:
    UnsignedConnection() = default;

    bool validateCredentials(const Credentials& c) {
        return bank.areValidCredentials(c);
    }

    Unique<SignedConnection> createConnection(const Credentials& c) {
        if( bank.areValidCredentials(c)) {

        }
    }
};
#endif //UPA_ATM_UNSIGNEDCONNECTION_H
