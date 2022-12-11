#include "backend/database/SqlQuery.h"
#include "backend/database/db_util.h"
#include "backend/database/db_category.h"
#include "backend/database/db_holder.h"
#include "backend/database/db_card.h"
#include "db_parent_relation.h"

void executeStatement(const QSqlDatabase &db, const char *stmttext) {
    SqlQuery query(db);
    query.exec(stmttext);
}

void populateDatabase(const QSqlDatabase &db) {
    qDebug() << "Populating database...";
    ullong cat1 = DBCategory::create({0, "Premium", 0}, db);
    qDebug() << "Created category with id" << cat1;
    ullong cat2 = DBCategory::create({0, "Universal", 0.05}, db);
    qDebug() << "Created category with id" << cat2;
    ullong hold1 = DBHolder::create({0, "Oleg", "Olegonka", "+380501234567"}, db);
    qDebug() << "Created holder with id" << hold1;
    ullong hold2 = DBHolder::create({0, "Vasyl", "Pupok", "+380507654321"}, db);
    qDebug() << "Created holder with id" << hold2;
    ullong hold3 = DBHolder::create({0, "Andrii", "Zahorulko", "+380507654321"}, db);
    qDebug() << "Created holder with id" << hold3;
    ullong hold4 = DBHolder::create({0, "Abraam", "Kitko", "+380507654321"}, db);
    qDebug() << "Created holder with id" << hold4;
    ullong card1 = DBCard::create({1234567891011121, 1234, QDate(2025, 6, 6), hold1, 100, cat1}, db);
    qDebug() << "Created card with id" << card1 << "pin:" << 1234;
    ullong card2 = DBCard::create({5168123412341234, 3221, QDate(2023, 7, 5), hold2, 50, cat2}, db);
    qDebug() << "Created card with id" << card2 << "pin:" << 3221;
    ullong card3 = DBCard::create({1234123412341234, 4567, QDate(2024, 5, 5), hold3, 1000, cat2}, db);
    qDebug() << "Created card with id" << card3 << "pin:" << 4567;
    ullong card4 = DBCard::create({5678567856785678, 4567, QDate(2024, 5, 5), hold4, 1000, cat2}, db);
    qDebug() << "Created card with id" << card4 << "pin:" << 4567;
    DBParentRelation::create({card3, card2, 1000}, db);
    DBParentRelation::create({card3, card4, 500}, db);
}

void initDatabase(const char *path, bool clear) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
    if (clear) {
        clearDatabase(db);
    }
    createDatabaseSchema(db);
    if (clear) {
        populateDatabase(db);
    }
}


void clearDatabase(const QSqlDatabase &db) {
    qDebug() << "Clearing tables...";
    SqlQuery query(db);
    query.exec("DROP TABLE IF EXISTS parent_relation");
    query.exec("DROP TABLE IF EXISTS bank_transaction");
    query.exec("DROP TABLE IF EXISTS card");
    query.exec("DROP TABLE IF EXISTS category");
    query.exec("DROP TABLE IF EXISTS holder");
}

inline void createTableCard(const QSqlDatabase &db) {
    static const char *stmttext = "CREATE TABLE IF NOT EXISTS card ("
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

inline void createTableCategory(const QSqlDatabase &db) {
    static const char *stmttext = "CREATE TABLE IF NOT EXISTS category ("
                                  "id INTEGER PRIMARY KEY,"
                                  "name TEXT UNIQUE,"
                                  "fee_rate REAL NOT NULL"
                                  ")";
    executeStatement(db, stmttext);
}

inline void createTableHolder(const QSqlDatabase &db) {
    static const char *stmttext = "CREATE TABLE IF NOT EXISTS holder ("
                                  "id INTEGER PRIMARY KEY,"
                                  "name TEXT NOT NULL,"
                                  "surname TEXT NOT NULL,"
                                  "phone_number TEXT NOT NULL"
                                  ")";
    executeStatement(db, stmttext);
}


inline void createTableParentRelation(const QSqlDatabase &db) {
    static const char *stmttext = "CREATE TABLE IF NOT EXISTS parent_relation ("
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

inline void createTableTransaction(const QSqlDatabase &db) {
    static const char *stmttext = "CREATE TABLE IF NOT EXISTS bank_transaction ("
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

void createDatabaseSchema(const QSqlDatabase &db) {
    qDebug() << "Creating database schema...";
    createTableHolder(db);
    createTableCard(db);
    createTableCategory(db);
    createTableParentRelation(db);
    createTableTransaction(db);
    qDebug() << "Created tables:" << db.tables();
}
