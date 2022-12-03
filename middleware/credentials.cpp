//
// Created by Andrii on 10/22/2022.
//
#include <QDebug>
#include "credentials.h"

Card::Card(ullong cardNumber) : _cardNumber(cardNumber) {

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

