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

ullong DBCategory::create(const DBCategory& category, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("INSERT INTO category (name, fee_rate)"
                  "VALUES (?,?)");
    int i = 0;
    query.bindValue(i++, category.getName().value());
    query.bindValue(i++, category.getFeeRate().value());
    query.exec();
    return query.lastInsertId().toLongLong();
}

Vector<DBCategory> DBCategory::selectAll(const QSqlDatabase &db)
{
    return selectAllT<DBCategory>("category", db);
}

DBCategory DBCategory::selectById(ullong id, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("SELECT * FROM category WHERE id = ?");
    query.bindValue(0, id);
    query.exec();
    if (query.next()) {
        return query.record();
    }
    throw DatabaseException("No category with given id."); // todo replace with more specific exception & add id to msg
}

void DBCategory::update(const DBCategory& category, const QSqlDatabase &db)
{
    if (!category.getId()) {
        throw DatabaseException("Cannot update category with unknown id.");
    }
    DBCategory old = DBCategory::selectById(category.getId().value(), db);
    category.to(old);
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

void DBCategory::remove(ullong id, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("DELETE FROM category WHERE id = ?");
    query.bindValue(0, id);
    query.exec();
}
