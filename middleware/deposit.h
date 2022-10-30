//
// Created by Andrii on 10/26/2022.
//

#ifndef BOOBLEEQUE_ATM_DEPOSIT_H
#define BOOBLEEQUE_ATM_DEPOSIT_H

#include "credentials.h"
#include "tariff.h"

class DepositRequest {
public:
    Credentials _from;
    uint _money;
    bool _afterTariff;

    inline DepositRequest(Credentials from, uint money, bool afterTariff) : _from(from), _money(money),
                                                                            _afterTariff(afterTariff) {}

    const Credentials &getFrom() const {
        return _from;
    }

    uint getMoney() const {
        return _money;
    }

    bool isAfterTariff() const {
        return _afterTariff;
    }

};

class DepositDetails {
    uint _money;
    Unique<Tariff> _tariff;

    inline DepositDetails(uint money, Unique<Tariff> tariff) : _money(money), _tariff(std::move(tariff)) {}

public:
    uint getMoney() const {
        return _money;
    }

    const Unique<Tariff> &getTariff() const {
        return _tariff;
    };
};

class WithdrawalRequest {
private:
    Credentials _from;
    uint _money;
    bool _afterTariff;
public:


    inline WithdrawalRequest(Credentials from, uint money, bool afterTariff) : _from(from), _money(money),
                                                                               _afterTariff(afterTariff) {}

    const Credentials &getFrom() const {
        return _from;
    }

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

    inline WithdrawalDetails(uint money, Unique<Tariff> tariff) : _money(money), _tariff(std::move(tariff)) {}

public:
    uint getMoney() const {
        return _money;
    }

    const Unique<Tariff> &getTariff() const {
        return _tariff;
    };
};

#endif //BOOBLEEQUE_ATM_DEPOSIT_H
