//
// Created by Andrii on 10/30/2022.
//

#ifndef UPA_ATM_CONNECTIONEXCEPTIONS_H
#define UPA_ATM_CONNECTIONEXCEPTIONS_H

#include "types.h"

class UnexpectedException{
private:
    String _message;
public:
    explicit UnexpectedException(const String &message) : _message(message) {}

    const String &message() const {
        return _message;
    }
};
#endif //UPA_ATM_CONNECTIONEXCEPTIONS_H
