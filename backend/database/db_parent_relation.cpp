#include "backend/database/db_parent_relation.h"
#include "backend/database/SqlQuery.h"
#include "backend/database/db_util.h"

void DBParentRelation::to(DBParentRelation& other) const
{
    if (_parent_card_id) {
        other.setParentCardId(_parent_card_id.value());
    }
    if (_child_card_id) {
        other.setChildCardId(_child_card_id.value());
    }
    if (_day_limit) {
        other.setDayLimit(_day_limit.value());
    }
}

ullong DBParentRelation::create(const DBParentRelation& parentRelation, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("INSERT INTO parent_relation (parent_card_id, child_card_id, day_limit)"
                  "VALUES (?,?,?)");
    int i = 0;
    query.bindValue(i++, parentRelation.getParentCardId().value());
    query.bindValue(i++, parentRelation.getChildCardId().value());
    query.bindValue(i++, parentRelation.getDayLimit() ?
                        parentRelation.getDayLimit().value() : QVariant());
    query.exec();
    return query.lastInsertId().toLongLong();
}

Vector<DBParentRelation> DBParentRelation::selectAll(const QSqlDatabase &db)
{
    return selectAllT<DBParentRelation>("parent_relation", db);
}

Vector<DBParentRelation> DBParentRelation::selectByParentId(ullong id, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("SELECT * FROM parent_relation WHERE parent_card_id = ?");
    query.bindValue(0, id);
    query.exec();
    Vector<DBParentRelation> result;
    while (query.next()) {
        result.push_back(query.record());
    }
    return result;
}

Optional<DBParentRelation> DBParentRelation::selectByChildId(ullong id, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("SELECT * FROM parent_relation WHERE child_card_id = ?");
    query.bindValue(0, id);
    query.exec();
    if (query.next()) {
        return query.record();
    }
    return Optional<DBParentRelation>();
}

void DBParentRelation::update(const DBParentRelation& parentRelation, const QSqlDatabase &db)
{
    if (!parentRelation.getChildCardId()) {
        throw DatabaseException("Cannot update parent relation with unknown child id.");
    }
    Optional<DBParentRelation> maybeOld = DBParentRelation::selectByChildId(parentRelation.getChildCardId().value(), db);
    if (!maybeOld.has_value()) {
        throw DatabaseException("no relation with given child id");
    }
    DBParentRelation old = maybeOld.value();
    parentRelation.to(old);
    SqlQuery query(db);
    query.prepare("UPDATE parent_relation SET "
                  "parent_card_id = ?,"
                  "day_limit = ?"
                  " WHERE child_card_id = ?");
    int i = 0;
    query.bindValue(i++, old.getParentCardId().value());
    query.bindValue(i++, old.getDayLimit().value());
    query.bindValue(i++, old.getChildCardId().value());
    query.exec();
}

void DBParentRelation::remove(ullong id, const QSqlDatabase &db)
{
    SqlQuery query(db);
    query.prepare("DELETE FROM parent_relation WHERE child_card_id = ?");
    query.bindValue(0, id);
    query.exec();
}
