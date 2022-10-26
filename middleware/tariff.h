//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_TARIFF_H
#define BOOBLEEQUE_ATM_TARIFF_H

#include "types.h"

class Tariff {
private:

    virtual String getTariff_v(uint) const = 0;

    virtual uint getLeftOver_v(uint) const = 0;

public:

    inline String getTariff(uint money) const { return getTariff_v(money); };

    inline uint getLeftOver(uint money) const { return getLeftOver_v(money); };

    Tariff() {}
    virtual ~Tariff() {}
};

//class PercentageTariff : public Tariff {
//private:
//    uint _percentage;
//
//public:
//    PercentageTariff()
//};
//
//class WholeTariff : public Tariff {
//private:
//    uint _money;
//    uint _loss;
//public:
//
//};

#endif //BOOBLEEQUE_ATM_TARIFF_H
