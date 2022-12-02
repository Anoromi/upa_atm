//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_TRANSFER_H
#define BOOBLEEQUE_ATM_TRANSFER_H

#include <utility>

#include "credentials.h"
#include "types.h"
#include "tariff.h"

class TransferDetails {
private:
    String _recipientName;
    Card _recipientCard;
    uint _money;
    Shared<Tariff> _tariff;
public:
    TransferDetails(
            String recipientName,
            const Card &recipientCard,
            const uint money,
            Shared<Tariff> tariff
    ) : _recipientName(std::move(recipientName)), _recipientCard(recipientCard), _money(money), _tariff(std::move(tariff)) {
    }

    const String &getRecipientName() const {
        return _recipientName;
    }

    const Card &getRecipientCard() const {
        return _recipientCard;
    }

    uint getMoney() const {
        return _money;
    }

    const Tariff &getTariff() const {
        return *_tariff.get();
    }
};

class TransferRequest {
public:
    TransferRequest(const Card &destination, uint money, bool afterTariff);

    const Card &getDestination() const;

    uint getMoney() const;

    bool isAfterTariff() const;

private:
    Card _destination;
    uint _money;
    bool _afterTariff;
public:
};

#endif //BOOBLEEQUE_ATM_TRANSFER_H
