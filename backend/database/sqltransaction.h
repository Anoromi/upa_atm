#ifndef SQLTRANSACTION_H
#define SQLTRANSACTION_H

#include <QSqlDatabase>
#include "backend/database/DatabaseException.h"
#include "qdebug.h"

class SqlTransaction
{
private:
    QSqlDatabase _db;
    bool _rollbacked;
public:
    SqlTransaction() : _db(QSqlDatabase::database()), _rollbacked(false)
    {
        if (!_db.transaction())
        {
            throw DatabaseException(_db.lastError().text().toLatin1());
        }
    }

    void rollback()
    {
        if (!_rollbacked) {
            _db.rollback();
            _rollbacked = true;
        }
    }

    ~SqlTransaction()
    {
        if (_rollbacked) return;
        if (!_db.commit())
        {
            qDebug() << "Transaction could not commit!";
            if (!_db.rollback()) {
                qCritical() << "Rollback was not done!!!";
            }
        }
    }

    operator const QSqlDatabase&() {
        if (_rollbacked) {
            throw DatabaseException("Transaction was rollbacked and thus invalid.");
        }
        return _db;
    }
};

#endif // SQLTRANSACTION_H
