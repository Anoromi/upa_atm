//
// Created by Andrii on 10/27/2022.
//

#ifndef UPA_ATM_EXCEPTIONS_H
#define UPA_ATM_EXCEPTIONS_H

#include "types.h"
#include "credentials.h"
#include <QDebug>

class BadMoney {
private:
    uint _requested;
    uint _available;
public:
    inline BadMoney(const uint requested, const uint &available) : _requested(requested), _available(available) {}

    uint getRequested() const {
        return _requested;
    }

    uint getAvailable() const {
        return _available;
    }

};

class BadCredentials {
private:
    Credentials _credentials;

public:
    explicit BadCredentials(const Credentials &credentials) : _credentials(credentials) {}

    const Credentials &getCredentials() const {
        return _credentials;
    }
};

	
class BlockedCard {
};

class UnexpectedException{
private:
    String _message;
public:
    explicit UnexpectedException(const String &message) : _message(message) {
        qDebug() << "Unexpected exception created with" << message;
    }

    const String &message() const {
        return _message;
    }
};

class BadRecipient {
public:
    explicit BadRecipient(const Card &card) : _card(card) {}

    const Card &getCard() const {
        return _card;
    }

private:
    Card _card;
};

#endif //UPA_ATM_EXCEPTIONS_H
