//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_TYPES_H
#define BOOBLEEQUE_ATM_TYPES_H

#include <string>
#include <memory>

using ushort = unsigned short;
using uint = unsigned int;
using ullong = unsigned long long;
using llong = long long;


using String = std::string;
template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T>
using Shared = std::shared_ptr<T>;

#endif //BOOBLEEQUE_ATM_TYPES_H
