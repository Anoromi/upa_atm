#include "backend/database/db_holder.h"
#include "backend/database/SqlQuery.h"
#include "backend/database/db_util.h"

void DBHolder::to(DBHolder& other) const
{
    if (_id) {
        other.setId(_id.value());
    }
    if (_name) {
        other.setName(_name.value());
    }
    if (_surname) {
        other.setSurname(_surname.value());
    }
    if (_phone_number) {
        other.setPhoneNumber(_phone_number.value());
    }
}

ullong DBHolder::create(const DBHolder& holder)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("INSERT INTO holder (name, surname, phone_number)"
                  "VALUES (?,?,?)");
    int i = 0;
    query.bindValue(i++, holder.getName().value());
    query.bindValue(i++, holder.getSurname().value());
    query.bindValue(i++, holder.getPhoneNumber().value());
    query.exec();
    return query.lastInsertId().toLongLong();
}

Vector<DBHolder> DBHolder::selectAll()
{
    return selectAllT<DBHolder>("holder");
}

DBHolder DBHolder::selectById(ullong id)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("SELECT * FROM holder WHERE id = ?");
    query.bindValue(0, id);
    query.exec();
    if (query.next()) {
        return query.record();
    }
    throw DatabaseException("No holder with given id."); // todo replace with more specific exception & add id to msg
}

void DBHolder::update(const DBHolder& holder)
{
    if (!holder.getId()) {
        throw DatabaseException("Cannot update category with unknown id.");
    }
    DBHolder old = DBHolder::selectById(holder.getId().value());
    holder.to(old);
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("UPDATE holder SET "
                  "name = ?,"
                  "surname = ?,"
                  "phone_number = ?"
                  " WHERE id = ?");
    int i = 0;
    query.bindValue(i++, old.getName().value());
    query.bindValue(i++, old.getSurname().value());
    query.bindValue(i++, old.getPhoneNumber().value());
    query.bindValue(i++, old.getId().value());
    query.exec();
}

void DBHolder::remove(ullong id)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("DELETE FROM holder WHERE id = ?");
    query.bindValue(0, id);
    query.exec();
}
