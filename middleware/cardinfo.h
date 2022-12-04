//
// Created by Andrii on 12/3/2022.
//

#ifndef UPA_ATM_CARD_INFO_H
#define UPA_ATM_CARD_INFO_H

#include "types.h"

class CardInfo {
public:
    CardInfo(const String &name, uint balance) : _name(name), _balance(balance) {}

    const String &getName() const {
        return _name;
    }

    uint getBalance() const {
        return _balance;
    }
private:
    String _name;
    uint _balance;


};

#endif //UPA_ATM_CARD_INFO_H
