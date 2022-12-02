#ifndef DB_UTIL_H
#define DB_UTIL_H

#include "middleware/types.h"
#include <QSqlDatabase>
#include "backend/database/SqlQuery.h"

void executeStatement(QSqlDatabase& db, const char* stmttext);
void initDatabase(const char* path, bool clear = false);
void clearDatabase();
void createDatabaseSchema();

//const char* getTableName(const char* classname);

template <typename Entity>
Vector<Entity> selectAllT(const char* table_name, const QSqlDatabase& db)
{
    SqlQuery query(db);
    query.exec(QString("SELECT * FROM ") + QString(table_name));
    Vector<Entity> resultset;
    while (query.next())
    {
        resultset.push_back(query.record());
    }
    return resultset;
}

#endif // DB_UTIL_H
