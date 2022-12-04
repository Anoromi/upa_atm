//
// Created by Andrii on 04.11.2022.
//

#ifndef UPA_ATM_BANKPROVIDER_H
#define UPA_ATM_BANKPROVIDER_H

#include "bank.h"
#include "backend/database/db_util.h"

class BankProvider {
private:
    static Bank* _bank;
public:
    static Bank& getBank() {
        if(_bank == nullptr) {
            // remove true to save data
            initDatabase("bank.db", true);
            _bank = new Bank();
        }
        return *_bank;
    }
};

#endif //UPA_ATM_BANKPROVIDER_H
