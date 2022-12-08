//
// Created by Andrii on 12/5/2022.
//

#ifndef UPA_ATM_TRANSACTION_H
#define UPA_ATM_TRANSACTION_H

#include <optional>
#include <backend/database/db_transaction.h>
#include "credentials.h"
#include "types.h"

class Transaction {
public:
    Transaction(const std::optional<Card> &sender, const std::optional<Card> &receiver, uint money, uint tariff)
            : _sender(sender), _receiver(receiver), _money(money), _tariff(tariff) {}

    Transaction(const DBTransaction &transaction)
            : _sender(transaction.getSenderId()),
              _receiver(transaction.getReceiverId()),
              _money(transaction.getAmount().value()),
              _tariff(transaction.getFee().value()) {}

    const std::optional<Card> &getSender() const {
        return _sender;
    }

    const std::optional<Card> &getReceiver() const {
        return _receiver;
    }

    uint getMoney() const {
        return _money;
    }

    uint getTariff() const {
        return _tariff;
    }

private:
    std::optional<Card> _sender;
    std::optional<Card> _receiver;
    uint _money;
    uint _tariff;
};

#endif //UPA_ATM_TRANSACTION_H
