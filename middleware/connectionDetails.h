//
// Created by Andrii on 11/6/2022.
//

#ifndef UPA_ATM_CONNECTIONDETAILS_H
#define UPA_ATM_CONNECTIONDETAILS_H

#include "credentials.h"

class ConnectionDetails {

private:
    Credentials _credentials;

public:
    ConnectionDetails(const Credentials &credentials) : _credentials(credentials) {}

    inline const Credentials &getCredentials() const {
        return _credentials;
    }
};

#endif //UPA_ATM_CONNECTIONDETAILS_H
