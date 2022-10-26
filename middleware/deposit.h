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

};

class DepositDetails {
    uint _money;
    Unique<Tariff> _tariff;

    inline DepositDetails(uint money, Unique<Tariff> tariff) : _money(money), _tariff(std::move(tariff)) {};
};

class WithdrawalRequest {
public:

    Credentials _from;
    uint _money;
    bool _afterTariff;

    inline WithdrawalRequest(Credentials from, uint money, bool afterTariff) : _from(from), _money(money),
                                                                               _afterTariff(afterTariff) {}
};

class WithdrawalDetails {
    uint _money;
    Unique<Tariff> _tariff;

    inline WithdrawalDetails(uint money, Unique<Tariff> tariff) : _money(money), _tariff(std::move(tariff)) {};
};

#endif //BOOBLEEQUE_ATM_DEPOSIT_H
