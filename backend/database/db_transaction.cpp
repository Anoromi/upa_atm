#include "backend/database/db_transaction.h"
#include "backend/database/db_util.h"
#include "backend/database/SqlQuery.h"

ullong DBTransaction::create(const DBTransaction& transaction)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("INSERT INTO bank_transaction (sender_id, receiver_id, amount, time, description)"
                  "VALUES (?,?,?,?,?)");
    int i = 0;
    query.bindValue(i++, transaction.getSenderId() ?
                        transaction.getSenderId().value() : QVariant());
    query.bindValue(i++, transaction.getReceiverId() ?
                        transaction.getReceiverId().value() : QVariant());
    query.bindValue(i++, transaction.getAmount().value());
    query.bindValue(i++, transaction.getTime().value());
    query.bindValue(i++, transaction.getDescription() ?
                        transaction.getDescription().value() : QVariant());
    query.exec();
    return query.lastInsertId().toLongLong();
}

Vector<DBTransaction> DBTransaction::selectAll()
{
    return selectAllT<DBTransaction>("bank_transaction");
}

DBTransaction DBTransaction::selectById(ullong id)
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.prepare("SELECT * FROM bank_transaction WHERE id = ?");
    query.bindValue(0, id);
    query.exec();
    if (query.next()) {
        return query.record();
    }
    throw DatabaseException("No transaction with given id."); // todo replace with more specific exception & add id to msg
}