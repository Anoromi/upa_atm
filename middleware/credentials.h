//
// Created by Andrii on 10/22/2022.
//

#ifndef BOOBLEEQUE_ATM_CREDENTIALS_H
#define BOOBLEEQUE_ATM_CREDENTIALS_H


#include <cstdlib>
#include "types.h"

class Card {
private:
    ullong _cardNumber;
public:
    class BadChar;
    class TooBigNumber;
    Card(ullong cardNumber);
    Card(char numbers[16]) ;
};


class Card::BadChar {
private:
    char _badChar;
public:
    inline BadChar(char badChar) : _badChar(badChar) {}
    inline char badChar() const {return _badChar; }
};

class Card::TooBigNumber {
private:
    ullong _badNumber;
public:
    inline TooBigNumber(ullong badNumber) : _badNumber(badNumber) {}
    inline ullong  badNumber() const {return _badNumber; }
};

class Pin {
private:
    ushort _pin;
public:
    class TooBigNumber;
    Pin(ushort pin);

};

class Pin::TooBigNumber {
private:
    ushort _badPin;
public:
    TooBigNumber(ushort badPin) : _badPin(badPin) {}
    inline unsigned  short  badPin() const {return _badPin;}
};

class Credentials {
private:
    Card _card;
    Pin _pin;
public:
    Credentials(Card card, Pin pin) : _card(card), _pin(pin) {}
    const Card& card() const {return _card;}
    const Pin& pin() const {return _pin;}
};

#endif //BOOBLEEQUE_ATM_CREDENTIALS_H
