//
// Created by Andrii on 12/5/2022.
//

#ifndef UPA_ATM_TRANSACTION_H
#define UPA_ATM_TRANSACTION_H

#include <optional>
#include "types.h"
#include "middleware/credentials.h"

class Transaction {
public:
    Transaction(const std::optional<Card> &sender, const std::optional<Card> &receiver, uint money, uint tariff)
            : _sender(sender), _receiver(receiver), money(money), tariff(tariff) {}


    uint getMoney() const {
        return money;
    }

    uint getTariff() const {
        return tariff;
    }

    const std::optional<Card> &getSender() const {
        return _sender;
    }

    const std::optional<Card> &getReceiver() const {
        return _receiver;
    }

private:
    std::optional<Card> _sender;
    std::optional<Card> _receiver;
    uint money;
    uint tariff;
};

#endif //UPA_ATM_TRANSACTION_H
