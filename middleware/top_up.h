//
// Created by Andrii on 12/4/2022.
//

#ifndef UPA_ATM_TOP_UP_H
#define UPA_ATM_TOP_UP_H

#include "types.h"


class TopUpRequest {
public:
    TopUpRequest(uint money, String mobileNumber) :
            _money(money),
            _phoneNumber(std::move(mobileNumber)) {}

    inline uint money() const { return _money; }


    inline const String &mobileNumber() const { return _phoneNumber; }

private:
    uint _money;
    String _phoneNumber;
};

#endif //UPA_ATM_TOP_UP_H
