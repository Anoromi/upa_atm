//
// Created by Andrii on 11/26/2022.
//

#ifndef DB_CARD_H
#define DB_CARD_H

#include "middleware/types.h"
#include <QDate>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QVariant>

class DBCard {
private:
    Optional<ullong> _number;
    Optional<short> _pin;
    Optional<QDate> _expiration_date;
    Optional<ullong> _holder_id;
    Optional<ullong> _balance;
    Optional<ullong> _category_id;
public:
    DBCard() {}
    DBCard( ullong number,
            short pin,
            const QDate& expiration_date,
            ullong holder_id,
            ullong balance,
            ullong category_id):
    _number(number),
    _pin(pin),
    _expiration_date(expiration_date),
    _holder_id(holder_id),
    _balance(balance),
    _category_id(category_id)
    {

    }
    DBCard(const QSqlRecord& record) :
        DBCard(
        (record.value("number").value<ullong>()),
        (record.value("pin").value<short>()),
        (record.value("expiration_date").value<QDate>()),
        (record.value("holder_id").value<ullong>()),
        (record.value("balance").value<ullong>()),
        (record.value("category_id").value<ullong>()))
    {}

    inline void setNumber(ullong number) {_number = number;}
    inline void setPin(short pin) {_pin = pin;}
    inline void setExpirationDate(const QDate& expiration_date)
        {_expiration_date = expiration_date;}
    inline void setHolderId(ullong holder_id) {_holder_id = holder_id;}
    inline void setBalance(ullong balance) {_balance = balance;}
    inline void setCategoryId(ullong category_id) {_category_id = category_id;}

    inline Optional<ullong> getNumber() const { return _number; }
    inline Optional<short> getPin() const { return _pin; }
    inline Optional<QDate> getExpirationDate() const { return _expiration_date; }
    inline Optional<ullong> getHolderId() const { return _holder_id; }
    inline Optional<ullong> getBalance() const { return _balance; }
    inline Optional<ullong> getCategoryId() const { return _category_id; }

    void to(DBCard& other) const;

    static ullong create(const DBCard& card, const QSqlDatabase& db = QSqlDatabase::database());
    static Vector<DBCard> selectAll(const QSqlDatabase& db = QSqlDatabase::database());
    static DBCard selectByNumber(ullong number, const QSqlDatabase& db = QSqlDatabase::database());
    static void update(const DBCard& card, const QSqlDatabase& db = QSqlDatabase::database());
    static void remove(ullong number, const QSqlDatabase& db = QSqlDatabase::database());
};

#endif // DB_CARD_H
