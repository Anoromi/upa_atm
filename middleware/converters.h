//
// Created by Andrii on 12/1/2022.
//

#ifndef UPA_ATM_CONVERTERS_H
#define UPA_ATM_CONVERTERS_H

#include <variant>
#include <QString>
#include "types.h"
#include "credentials.h"

std::variant<uint, String> parseMoney(const String &text);

String moneyToString(uint money);

std::variant<Card, String> parseCard(const String &text);

QString stringToQ(const String& s);
#endif //UPA_ATM_CONVERTERS_H
