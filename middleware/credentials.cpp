//
// Created by Andrii on 10/22/2022.
//
#include <QDebug>
#include "credentials.h"
#include "exceptions.h"


//Card::Card(char *numbers) : _cardNumber(0) {
//    for (size_t s = 0; s < 16; s++) {
//        char c = numbers[s];
//        if (c < '0' || c > '9')
//            throw Card::BadChar(c);
//        _cardNumber *= 10;
//        _cardNumber += c - '0';
//    }
//}

Card::Card(ullong cardNumber) : _cardNumber(cardNumber) {

//    if(1234123412341234uLL > 9999999999999999uLL)
//        throw UnexpectedException(L"");
    if(cardNumber > 9999999999999999ULL) {
        qDebug() << cardNumber;
        throw Card::TooBigNumber(cardNumber);
    }
}

ullong Card::getCardNumber() const {
    return _cardNumber;
}

Pin::Pin(ushort pin) : _pin(pin) {
    if(pin > 9999)
        throw Pin::TooBigNumber(pin);
}

ushort Pin::getPin() const {
    return _pin;
}

ChildCard::ChildCard(const String &name, const Card &card) : _name(name), _card(card) {}

