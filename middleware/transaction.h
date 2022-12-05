//
// Created by Andrii on 12/5/2022.
//

#ifndef UPA_ATM_TRANSACTION_H
#define UPA_ATM_TRANSACTION_H

#include <optional>
#include "types.h"

class Transaction {
public:
    Transaction(const std::optional<ullong> &sender, const std::optional<ullong> &receiver, uint money, uint tariff)
            : _sender(sender), _receiver(receiver), money(money), tariff(tariff) {}

    const std::optional<ullong> &getSender() const {
        return _sender;
    }

    const std::optional<ullong> &getReceiver() const {
        return _receiver;
    }

    uint getMoney() const {
        return money;
    }

    uint getTariff() const {
        return tariff;
    }

private:
    std::optional<ullong> _sender;
    std::optional<ullong> _receiver;
    uint money;
    uint tariff;
};

#endif //UPA_ATM_TRANSACTION_H
