#include "backend/database/db_category.h"
#include "backend/database/SqlQuery.h"
#include "backend/database/db_util.h"

void DBCategory::to(DBCategory& other) const
{
    if (_id) {
        other.setId(_id.value());
    }
    if (_name) {
        other.setName(_name.value());
    }
    if (_fee_rate) {
        other.setFeeRate(_fee_rate.value());
    }
}

ullong DBCategory::create(const DBCategory& category)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("INSERT INTO category (name, fee_rate)"
                  "VALUES (?,?)");
    int i = 0;
    query.bindValue(i++, category.getName().value());
    query.bindValue(i++, category.getFeeRate().value());
    query.exec();
    return query.lastInsertId().toLongLong();
}

Vector<DBCategory> DBCategory::selectAll()
{
    return selectAllT<DBCategory>("category");
}

DBCategory DBCategory::selectById(ullong id)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("SELECT * FROM category WHERE id = ?");
    query.bindValue(0, id);
    query.exec();
    if (query.next()) {
        return query.record();
    }
    throw DatabaseException("No category with given id."); // todo replace with more specific exception & add id to msg
}

void DBCategory::update(const DBCategory& category)
{
    if (!category.getId()) {
        throw DatabaseException("Cannot update category with unknown id.");
    }
    DBCategory old = DBCategory::selectById(category.getId().value());
    category.to(old);
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("UPDATE category SET "
                  "name = ?,"
                  "fee_rate = ?"
                  " WHERE id = ?");
    int i = 0;
    query.bindValue(i++, old.getName().value());
    query.bindValue(i++, old.getFeeRate().value());
    query.bindValue(i++, old.getId().value());
    query.exec();
}

void DBCategory::remove(ullong id)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("DELETE FROM category WHERE id = ?");
    query.bindValue(0, id);
    query.exec();
}
