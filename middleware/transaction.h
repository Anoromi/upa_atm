//
// Created by Andrii on 12/5/2022.
//

#ifndef UPA_ATM_TRANSACTION_H
#define UPA_ATM_TRANSACTION_H

#include <optional>
#include <backend/database/db_transaction.h>
#include "types.h"

class Transaction {
public:
    Transaction(const std::optional<ullong> &sender,
                const std::optional<ullong> &receiver,
                uint money, uint tariff, const String& description)
            : _sender(sender), _receiver(receiver), _money(money), _tariff(tariff), _description(description) {}

    Transaction(const DBTransaction &transaction)
        : _sender(transaction.getSenderId()),
          _receiver(transaction.getReceiverId()),
          _money(transaction.getAmount().value()),
          _tariff(transaction.getFee().value()),
          _description(transaction.getDescription().value().toStdWString()),
          _time(transaction.getTime().value())
    {}

    const std::optional<ullong> &getSender() const {
        return _sender;
    }

    const std::optional<ullong> &getReceiver() const {
        return _receiver;
    }

    uint getMoney() const {
        return _money;
    }

    uint getTariff() const {
        return _tariff;
    }

    const String& getDescription() const {
        return _description;
    }

    const QDateTime& getTime() const {
        return _time;
    }

private:
    Optional<ullong> _sender;
    Optional<ullong> _receiver;
    uint _money;
    uint _tariff;
    String _description;
    QDateTime _time;
};

#endif //UPA_ATM_TRANSACTION_H
