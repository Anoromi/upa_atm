//
// Created by Andrii on 12/1/2022.
//

#include "converters.h"
#include <sstream>

std::variant<uint, String> parseMoney(const String &text) {
    if (text.length() == 0)
        return L"Потрібно ввести хоаб одну цифру";
    int i = 0;
    for (; text[i] != '.' && i < text.length(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            return L"Неправильний символ у числі";
        }
    }
    if (i + 3 == text.length() && text[i] == '.') {
        i++;
        for (; i < text.length(); i++) {
            if (text[i] < '0' || text[i] > '9') {
                return L"Неправильний символ у числі";
            }
        }
    } else if (text[i] == '.') {
        return L"Потрібно ввести копійки після крапки";
    }
    return static_cast<uint>(std::stod(text) * 100);
}

std::variant<Card, String> parseCard(const String &text) {
    if (text.length() != 16)
        return L"Картка повинна містити 16 символів";
    ullong card = 0;
    for (int i = 0; i < 16; i++) {
        if (text[i] < '0' || text[i] > '9')
            return L"Неправильний символ у карці";
    }
    card = std::stoll(text);
    return card;
}

String moneyToString(const uint money) {
    if (money < 10) {
        return (std::wostringstream() << ".0" << money).str();
    }
    if (money < 100) {
        return (std::wostringstream() << "0" << "." << money).str();
    }
    uint curMoney = money;
    std::wostringstream output = std::wostringstream();
    output << curMoney % 10;
    curMoney /= 10;
    output << curMoney % 10 << '.';
    curMoney /= 10;
    while (curMoney > 0) {
        output << curMoney % 10;
        curMoney /= 10;
    }

    String res = output.str();
    for (int i = 0; i < res.length() / 2; i++) {
        res[i] += res[res.length() - i - 1];
        res[res.length() - i - 1] = res[i] - res[res.length() - i - 1];
        res[i] -= res[res.length() - i - 1];
    }
    return std::move(res);
}

QString stringToQ(const String &s) {
    return QString::fromWCharArray(s.c_str());
}

std::variant<Pin, String> parsePin(const String &text) {
    if (text.length() != 4)
        return L"Пін повинен складатися з 4 цифр";

    ushort result = 0;
    for (int i = 0; i < 4; i++) {
        if (text[i] < '0' || text[i] > '9')
            return L"Пін повинен містити лише цифри";
    }
    result = std::stoi(text);
    return Pin(result);
}

String ullongToString(ullong u) {
    return (std::wostringstream() << u).str();
}
