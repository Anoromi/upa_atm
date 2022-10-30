//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_TRANSFER_H
#define BOOBLEEQUE_ATM_TRANSFER_H

#include "credentials.h"
#include "types.h"
#include "tariff.h"

class TransferDetails {
private:
    String _recipientName;
    Card _recipientCard;
    uint _money;
    Unique<Tariff> _tariff;
public:
    TransferDetails(
            const String &recipientName,
            const Card &recipientCard,
            const uint money,
            Unique<Tariff> tariff
    ) : _recipientName(recipientName), _recipientCard(recipientCard), _money(money), _tariff(std::move(tariff)) {
    }
};

class TransferRequest {
private:
    Card _destination;
    uint _money;
    bool _afterTariff;
public:
//    TransferRequest(
//            Credentials from,
//            Card destination,
//            uint money,
//            bool afterTariff
//    ) : _from(from), _destination(destination), _money(money), _afterTariff(afterTariff) {}
//
//    Credentials& from() {return _from;}
//    Card& destination() {return _destination;}
};

#endif //BOOBLEEQUE_ATM_TRANSFER_H
