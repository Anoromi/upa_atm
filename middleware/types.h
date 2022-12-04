//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_TYPES_H
#define BOOBLEEQUE_ATM_TYPES_H

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <set>

using ushort = unsigned short;
using uint = unsigned int;
using ullong = unsigned long long;
using llong = long long;


using String = std::wstring;
template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T>
using Vector = std::vector<T>;

template<typename T>
using Optional = std::optional<T>;

template <typename T>
using Set = std::set<T>;

#endif //BOOBLEEQUE_ATM_TYPES_H
