//
// Created by Andrii on 11/26/2022.
//

#ifndef UPA_ATM_USER_CARD_H
#define UPA_ATM_USER_CARD_H

#include "middleware/types.h"

class UserCard {
private:
    long number;
    short pin;
    ullong expiration_date;
    ullong holder_id;
    ullong balance;
    ullong category_id;
public:

};

#endif //UPA_ATM_USER_CARD_H
