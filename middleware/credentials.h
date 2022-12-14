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

    ullong getCardNumber() const;
};


class Card::BadChar {
private:
    char _badChar;
public:
    inline BadChar(char badChar) : _badChar(badChar) {}

    inline char badChar() const { return _badChar; }
};

class Card::TooBigNumber {
private:
    ullong _badNumber;
public:
    inline TooBigNumber(ullong badNumber) : _badNumber(badNumber) {}

    inline ullong badNumber() const { return _badNumber; }
};

class Pin {
private:
    ushort _pin;
public:
    class TooBigNumber;

    Pin(ushort pin);

    ushort getPin() const;

};

class Pin::TooBigNumber {
private:
    ushort _badPin;
public:
    TooBigNumber(ushort badPin) : _badPin(badPin) {}

    inline unsigned short badPin() const { return _badPin; }
};

class Credentials {
private:
    Card _card;
    Pin _pin;
public:
    Credentials(Card card, Pin pin) : _card(card), _pin(pin) {}

    inline const Card &card() const { return _card; }

    inline const Pin &pin() const { return _pin; }
};

template<typename T>
class Authorized {
private:
    Credentials _credentials;
    T _value;
public:
    inline Authorized(const Credentials &credentials, const T &value) : _credentials(credentials), _value(value) {}

    inline const Credentials &credentials() const { return _credentials; }

    inline const T &value() const { return _value; }

    inline T &value() { return _value; }
};

class ChildCard {
public:
    ChildCard(const String &name, const Card &card);

    inline const String &getName() const {
        return _name;
    }

    inline const Card &getCard() const {
        return _card;
    }


private:
    String _name;
    Card _card;
};

#endif //BOOBLEEQUE_ATM_CREDENTIALS_H
