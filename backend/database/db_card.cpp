#include "backend/database/db_card.h"
#include "backend/database/SqlQuery.h"
#include "backend/database/db_util.h"

void DBCard::to(DBCard& other) const {
    if (_number) {
        other.setNumber(_number.value());
    }
    if (_pin) {
        other.setPin(_pin.value());
    }
    if (_expiration_date) {
        other.setExpirationDate(_expiration_date.value());
    }
    if (_holder_id) {
        other.setHolderId(_holder_id.value());
    }
    if (_balance) {
        other.setBalance(_balance.value());
    }
    if (_category_id) {
        other.setCategoryId(_category_id.value());
    }
}

ullong DBCard::create(const DBCard& card, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("INSERT INTO card (number, pin, expiration_date, holder_id, balance, category_id)"
                  "VALUES (?,?,?,?,?,?)");
    query.bindValue(0,card.getNumber().value());
    query.bindValue(1,card.getPin().value());
    query.bindValue(2,card.getExpirationDate().value());
    query.bindValue(3,card.getHolderId().value());
    query.bindValue(4,card.getBalance().value());
    query.bindValue(5,card.getCategoryId().value());
    query.exec();
    return query.lastInsertId().toLongLong();
}

Vector<DBCard> DBCard::selectAll(const QSqlDatabase &db)
{
    return selectAllT<DBCard>("card", db);
}

DBCard DBCard::selectByNumber(ullong number, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("SELECT * FROM card WHERE number = ?");
    query.bindValue(0,number);
    query.exec();
    if (query.next()) {
        return query.record();
    }
    throw DatabaseException("No card with given id."); // todo replace with more specific exception & add id to msg
}

void DBCard::update(const DBCard& card, const QSqlDatabase &db)
{
    if (!card.getNumber()) {
        throw DatabaseException("Cannot update card with unknown id.");
    }
    DBCard old = DBCard::selectByNumber(card.getNumber().value(), db);
    card.to(old);
    SqlQuery query(db);
    query.prepare("UPDATE card SET "
                  "pin = ?,"
                  "expiration_date = ?,"
                  "holder_id = ?,"
                  "balance = ?,"
                  "category_id = ? WHERE number = ?");
    int i = 0;
    query.bindValue(i++, old.getPin().value());
    query.bindValue(i++, old.getExpirationDate().value());
    query.bindValue(i++, old.getHolderId().value());
    query.bindValue(i++, old.getBalance().value());
    query.bindValue(i++, old.getCategoryId().value());
    query.bindValue(i++, old.getNumber().value());
    query.exec();
}

void DBCard::remove(ullong number, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("DELETE FROM card WHERE number = ?");
    query.bindValue(0, number);
    query.exec();
}
