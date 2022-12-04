//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_TARIFF_H
#define BOOBLEEQUE_ATM_TARIFF_H

#include "types.h"
#include <sstream>
#include "middleware/exceptions.h"

class Tariff {
private:

    virtual String getTariff_v(uint) const = 0;

    virtual uint getFee_v(uint) const = 0;

public:
    inline String getTariff(uint money) const { return getTariff_v(money); };
    inline uint getFee(uint money) const { return getFee_v(money); }
    inline uint getLeftOver(uint money) const { return money - getFee(money); };

    inline Tariff() = default;
    virtual ~Tariff() {}
};

class PercentageTariff : public Tariff {
private:
    double _percentage;

public:
    explicit inline PercentageTariff(double percentage) : Tariff(), _percentage(percentage)
    {
        if(percentage < 0 || percentage >= 1) {
            throw UnexpectedException(L"PercentageTariff: wrong %");
        }
    }

private:
    inline String getTariff_v(uint money) const override {
        std::wstringstream st;
        st << _percentage;
        return st.str();
    }

    inline uint getFee_v(uint money) const override {
        return money * (uint) (_percentage * 100) / 100;
    }
};

class WholeTariff : public Tariff {
private:
    uint _loss;
public:
    explicit inline WholeTariff(uint loss) : _loss(loss) {}

private:
    inline String getTariff_v(uint money) const override {
        std::wstringstream st;
        st << _loss;
        return st.str();
    }

    inline uint getFee_v(uint money) const override {
        if(money < _loss) {
            return money;
        }
        return _loss;
    }
};

#endif //BOOBLEEQUE_ATM_TARIFF_H
