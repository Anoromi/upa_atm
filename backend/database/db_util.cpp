#include "backend/database/SqlQuery.h"
#include "backend/database/db_util.h"

static const char* dbcard = "class DBCard";


//const char* getTableName(const char* classname)
//{
//    if (strcmp(dbcard, classname) == 0) return "card";
//    if (strcmp("class DBCategory", classname) == 0) return "category";
//    if (strcmp("class DBHolder", classname) == 0) return "holder";
//    if (strcmp("class DBParentRelation", classname) == 0) return "parent_relation";
//    if (strcmp("class DBTransaction", classname) == 0) return "bank_transaction";
//    throw DatabaseException("getTableName: Unknown entity class.");
//};

void executeStatement(QSqlDatabase& db, const char* stmttext)
{
    SqlQuery query(db);
    query.exec(stmttext);
}

void initDatabase(const char* path, bool clear)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (clear) {
        clearDatabase();
    }
    createDatabaseSchema();
}

void clearDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);
    query.exec("DROP TABLE IF EXISTS parent_relation");
    query.exec("DROP TABLE IF EXISTS bank_transaction");
    query.exec("DROP TABLE IF EXISTS card");
    query.exec("DROP TABLE IF EXISTS category");
    query.exec("DROP TABLE IF EXISTS holder");
}

inline void createTableCard(QSqlDatabase& db)
{
    static const char* stmttext = "CREATE TABLE IF NOT EXISTS card ("
                               "number INTEGER NOT NULL,"
                               "pin INTEGER NOT NULL,"
                               "expiration_date TEXT NOT NULL,"
                               "holder_id INTEGER NOT NULL,"
                               "balance INTEGER NOT NULL,"
                               "category_id INTEGER NOT NULL,"
                               "PRIMARY KEY (number),"
                               "FOREIGN KEY (holder_id) REFERENCES holder (id)"
                               "ON DELETE NO ACTION ON UPDATE CASCADE,"
                               "FOREIGN KEY (category_id) REFERENCES category (id)"
                               "ON DELETE NO ACTION ON UPDATE CASCADE"
                               ")";
    executeStatement(db, stmttext);
}

inline void createTableCategory(QSqlDatabase& db)
{
    static const char* stmttext = "CREATE TABLE IF NOT EXISTS category ("
                                  "id INTEGER PRIMARY KEY,"
                                  "name TEXT UNIQUE,"
                                  "fee_rate REAL NOT NULL"
                                  ")";
    executeStatement(db, stmttext);
}

inline void createTableHolder(QSqlDatabase& db)
{
    static const char* stmttext = "CREATE TABLE IF NOT EXISTS holder ("
                                  "id INTEGER PRIMARY KEY,"
                                  "name TEXT NOT NULL,"
                                  "surname TEXT NOT NULL,"
                                  "phone_number TEXT NOT NULL"
                                  ")";
    executeStatement(db, stmttext);
}


inline void createTableParentRelation(QSqlDatabase& db)
{
    static const char* stmttext = "CREATE TABLE IF NOT EXISTS parent_relation ("
                                  "parent_card_id INTEGER NOT NULL,"
                                  "child_card_id INTEGER NOT NULL,"
                                  "day_limit INTEGER,"
                                  "PRIMARY KEY (child_card_id),"
                                  "FOREIGN KEY (parent_card_id) REFERENCES card (number)"
                                  "ON DELETE CASCADE ON UPDATE CASCADE,"
                                  "FOREIGN KEY (child_card_id) REFERENCES card (number)"
                                  "ON DELETE CASCADE ON UPDATE CASCADE"
                                  ")";
    executeStatement(db, stmttext);
}

inline void createTableTransaction(QSqlDatabase& db)
{
    static const char* stmttext = "CREATE TABLE IF NOT EXISTS bank_transaction ("
                                  "id INTEGER PRIMARY KEY,"
                                  "sender_id INTEGER,"
                                  "receiver_id INTEGER,"
                                  "amount INTEGER NOT NULL,"
                                  "fee INTEGER NOT NULL,"
                                  "time TEXT NOT NULL,"
                                  "description TEXT,"
                                  "FOREIGN KEY (sender_id) REFERENCES card (number)"
                                  "ON DELETE RESTRICT ON UPDATE CASCADE,"
                                  "FOREIGN KEY (receiver_id) REFERENCES card (number)"
                                  "ON DELETE RESTRICT ON UPDATE CASCADE"
                                  ")";
    executeStatement(db, stmttext);
}

void createDatabaseSchema()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    createTableHolder(db);
    createTableCard(db);
    createTableCategory(db);
    createTableParentRelation(db);
    createTableTransaction(db);
    qDebug() << db.tables();
    db.close();
}

template<typename T>
void create(T& entity)
{

}
