//
// Created by Andrii on 10/26/2022.
//

#ifndef BOOBLEEQUE_ATM_DEPOSIT_H
#define BOOBLEEQUE_ATM_DEPOSIT_H

#include "credentials.h"
#include "tariff.h"

class DepositRequest {
public:
    uint _money;

    inline DepositRequest(uint money) : _money(money) {}

    uint getMoney() const {
        return _money;
    }

};

class DepositDetails {
private:
    uint _money;
    Unique<Tariff> _tariff;
    uint _previousBalance;

public:
    DepositDetails(uint money, Unique<Tariff> &&tariff, uint resultingBalance)
            : _money(money),
              _tariff(std::move(tariff)),
              _previousBalance(resultingBalance) {}

    uint getMoney() const {
        return _money;
    }

    const Tariff &getTariff() const {
        return *_tariff.get();
    }

    uint getPreviousBalance() const {
        return _previousBalance;
    }

};

class WithdrawalRequest {
private:
    uint _money;
    bool _afterTariff;
public:


    inline WithdrawalRequest(uint money, bool afterTariff)
            : _money(money),
              _afterTariff(afterTariff) {}


    uint getMoney() const {
        return _money;
    }

    bool isAfterTariff() const {
        return _afterTariff;
    }
};

class WithdrawalDetails {
private:
    uint _money;
    Unique<Tariff> _tariff;

public:
    inline WithdrawalDetails(uint money, Unique<Tariff> tariff) : _money(money), _tariff(std::move(tariff)) {}

    uint getMoney() const {
        return _money;
    }

    const Unique<Tariff> &getTariff() const {
        return _tariff;
    };
};

#endif //BOOBLEEQUE_ATM_DEPOSIT_H
