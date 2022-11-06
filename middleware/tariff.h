//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_TARIFF_H
#define BOOBLEEQUE_ATM_TARIFF_H

#include "types.h"
#include <sstream>

class Tariff {
private:

    virtual String getTariff_v(uint) const = 0;

    virtual uint getLeftOver_v(uint) const = 0;

public:

    inline String getTariff(uint money) const { return getTariff_v(money); };

    inline uint getLeftOver(uint money) const { return getLeftOver_v(money); };

    inline Tariff() {}
    virtual ~Tariff() {}
};

class PercentageTariff : public Tariff {
private:
    double _percentage;

public:
    explicit inline PercentageTariff(double percentage) : Tariff(), _percentage(percentage) {}

private:
    inline virtual String getTariff_v(uint money) const override {
        std::ostringstream st;
        st << _percentage;
        return st.str();
    }

    inline virtual uint getLeftOver_v(uint money) const override {
        return money * (uint)(_percentage * 100) / 100;
    }
};

class WholeTariff : public Tariff {
private:
    uint _loss;
public:
    explicit inline WholeTariff(uint loss) : _loss(loss) {}

private:
    inline String getTariff_v(uint money) const override {
        std::ostringstream st;
        st << _loss;
        return st.str();
    }

    inline uint getLeftOver_v(uint money) const override {
        return money - _loss;
    }
};

#endif //BOOBLEEQUE_ATM_TARIFF_H
