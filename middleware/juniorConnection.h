//
// Created by Andrii on 10/30/2022.
//

#ifndef UPA_ATM_JUNIORCONNECTION_H
#define UPA_ATM_JUNIORCONNECTION_H

#include "signedConnection.h"

class JuniorConnection : public SignedConnection {
public:
    JuniorConnection(Credentials c) : SignedConnection(c) {}

    void getLinkageDetails() {

    }
    void linkToParentCard() {

    }

};

#endif //UPA_ATM_JUNIORCONNECTION_H
