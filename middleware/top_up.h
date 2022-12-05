//
// Created by Andrii on 12/4/2022.
//

#ifndef UPA_ATM_TOP_UP_H
#define UPA_ATM_TOP_UP_H

#include "types.h"


class TopUpRequest {
public:
    TopUpRequest(uint money, bool isAfterTariff) : _money(money),
                                                   _isAfterTariff(isAfterTariff) {}

private:
    uint _money;
    bool _isAfterTariff;
}

#endif //UPA_ATM_TOP_UP_H
