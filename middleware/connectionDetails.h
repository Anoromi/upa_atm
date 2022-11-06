//
// Created by Andrii on 11/6/2022.
//

#ifndef UPA_ATM_CONNECTIONDETAILS_H
#define UPA_ATM_CONNECTIONDETAILS_H

#include "credentials.h"

class ConnectionDetails {

private:
    Credentials _credentials;
    bool _isParentCard;

public:
    ConnectionDetails(const Credentials &credentials, bool isParentCard) : _credentials(credentials),
                                                                           _isParentCard(isParentCard) {}

    inline const Credentials &getCredentials() const {
        return _credentials;
    }

    inline bool isParentCard() const {
        return _isParentCard;
    }
};

#endif //UPA_ATM_CONNECTIONDETAILS_H
