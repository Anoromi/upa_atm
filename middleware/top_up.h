//
// Created by Andrii on 12/4/2022.
//

#ifndef UPA_ATM_TOP_UP_H
#define UPA_ATM_TOP_UP_H

#include "types.h"


class TopUpRequest {
public:
    TopUpRequest(uint money, bool isAfterTariff, const String& mobileNumber) :
        _money(money),
        _isAfterTariff(isAfterTariff),
      _mobileNumber(mobileNumber)
    {}

    inline uint money() const {return _money; }
    inline bool isAfterTariff() const { return _isAfterTariff; }
    inline const String& mobileNumber() const { return _mobileNumber; }
private:
    uint _money;
    bool _isAfterTariff;
    String _mobileNumber;
};

#endif //UPA_ATM_TOP_UP_H
