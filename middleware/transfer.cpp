//
// Created by Andrii on 10/22/2022.
//

#include "transfer.h"

const Card &TransferRequest::getDestination() const {
    return _destination;
}

uint TransferRequest::getMoney() const {
    return _money;
}

bool TransferRequest::isAfterTariff() const {
    return _afterTariff;
}
