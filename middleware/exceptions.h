//
// Created by Andrii on 10/27/2022.
//

#ifndef UPA_ATM_EXCEPTIONS_H
#define UPA_ATM_EXCEPTIONS_H

#include "types.h"
#include "credentials.h"

class BadMoney {
private:
    uint _requested;
    uint _available;
public:
    inline BadMoney(const uint requested, const uint &available) : _requested(requested), _available(available) {}

    uint getRequested() const {
        return _requested;
    }

    uint getAvailable() const {
        return _available;
    }

};

class BadPassword {
    Credentials _credentials;
};

#endif //UPA_ATM_EXCEPTIONS_H
