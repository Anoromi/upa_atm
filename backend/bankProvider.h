//
// Created by Andrii on 04.11.2022.
//

#ifndef UPA_ATM_BANKPROVIDER_H
#define UPA_ATM_BANKPROVIDER_H

#include "bank.h"

class BankProvider {
private:
    static Bank* _bank;
public:
    static Bank& getBank() {
        if(_bank == nullptr) {

        }
        throw std::exception();
    }
};

#endif //UPA_ATM_BANKPROVIDER_H
