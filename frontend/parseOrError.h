//
// Created by Andrii on 12/2/2022.
//

#ifndef UPA_ATM_PARSEORERROR_H
#define UPA_ATM_PARSEORERROR_H

#include <variant>
#include "middleware/types.h"
#include "error_message.h"

template<typename T>
bool errorOnString(const std::variant<T, String> &value) {
    if (value.index() == 1) {
        showErrorMessage(std::get<String>(value));
        return true;
    }
    return false;
}

#endif //UPA_ATM_PARSEORERROR_H
