#ifndef SQLQUERY_H
#define SQLQUERY_H

#include "backend/database/DatabaseException.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>

class SqlQuery {
private:
    const QSqlDatabase& _db;
    QSqlQuery _query;
public:
    SqlQuery(const QSqlDatabase& db) : _db(db), _query(db) {}
    SqlQuery(const SqlQuery&) = delete;
    SqlQuery& operator=(const SqlQuery&) = delete;

    inline void exec()
    {
        if (!_query.isSelect())
        {
            enableFK();
        }
        if (!_query.exec()) {
            throw SqlQueryException(_query.lastQuery(), _query.lastError(), "SqlQuery");
        }
    }

    inline void exec(const QString& query)
    {
        if (!_query.isSelect())
        {
            enableFK();
        }
        if (!_query.exec(query)) {
            throw SqlQueryException(_query.lastQuery(), _query.lastError(), "SqlQuery");
        }
    }

    inline QSqlRecord record() const
    {
        return _query.record();
    }

    inline bool next()
    {
        return _query.next();
    }

    inline void prepare(const QString& query)
    {
        if (!_query.prepare(query))
        {
            throw SqlQueryException(_query.lastQuery(), _query.lastError(), "SqlQuery");
        }
    }

    inline void bindValue(const QString& placeholder, const QVariant& val,
                   QSql::ParamType type = QSql::In)
    {
        return _query.bindValue(placeholder, val, type);
    }

    inline void bindValue(int pos, const QVariant& val, QSql::ParamType type = QSql::In)
    {
        return _query.bindValue(pos, val, type);
    }

    inline QVariant value(int i) const
    {
        return _query.value(i);
    }

    inline QVariant value(const QString& name) const
    {
        return _query.value(name);
    }

    inline QVariant lastInsertId() const
    {
        return _query.lastInsertId();
    }

private:
    inline void enableFK() const
    {
        QSqlQuery query(_db);
        query.exec("PRAGMA foreign_keys = ON");
    }
};

#endif // SQLQUERY_H
