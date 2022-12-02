#include "login.h"

#include "backend/database/DatabaseException.h"
#include "backend/database/db_card.h"
#include "backend/database/db_category.h"
#include "backend/database/db_holder.h"
#include "backend/database/db_parent_relation.h"
#include "backend/database/db_transaction.h"
#include "navigationwindow.h"


#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <vector>
#include "backend/database/SqlQuery.h"
#include "backend/database/sqltransaction.h"
#include "backend/database/db_util.h"
void selectAllHolders()
{
    Vector<DBHolder> holders = DBHolder::selectAll();
    for (DBHolder& h : holders)
    {
        qDebug() << h.getId().value() << " " <<
                    h.getName().value() << " " <<
                    h.getSurname().value() << " " <<
                    h.getPhoneNumber().value();
    }
};

void transactionTest()
{
    {
        SqlTransaction transaction;
        qDebug() << "holders:" << DBHolder::selectAll().size();
        qDebug() << "cards:" << DBCard::selectAll().size();
        try {
            DBHolder::create(DBHolder(0, "holder_test", "holder_test", "test number"), transaction);
            DBCard::create(DBCard(1234123412341234, 1234, QDate(), 3, 1000,1), transaction);
        } catch (...) {
            qDebug() << "rollback!!";
            transaction.rollback();
        }
        qDebug() << "holders:" << DBHolder::selectAll().size();
        qDebug() << "cards:" << DBCard::selectAll().size();
        try {
            DBHolder::create(DBHolder(0, "holder_test", "holder_test", "test number"), transaction);
        } catch(...) {
            qDebug() << "we cannot use this transaction!";
        }
    }
}

void dbtest()
{
    initDatabase("bank.db", true);
    selectAllHolders();
    QSqlDatabase db = QSqlDatabase::database();
    SqlQuery query(db);

    DBHolder holder;
    holder.setName("Oleg");
    holder.setSurname("olegovich");
    holder.setPhoneNumber("+3800505050505");
    DBHolder::create(holder);
    DBCategory::create(DBCategory(0, "Premium", 0));
    DBCard::create(DBCard(5111050540403030, 5151, QDate(2022,12,31), 1, 5000, 1));
    selectAllHolders();
    for (const DBCard& card : DBCard::selectAll()) {
        qDebug() << card.getNumber().value() <<
                    card.getPin().value() <<
                    card.getExpirationDate().value() <<
                    card.getHolderId().value() <<
                    card.getBalance().value() <<
                    card.getCategoryId().value();
    }
    DBCard card = DBCard::selectByNumber(5111050540403030);
    qDebug() << card.getNumber().value() <<
                card.getPin().value() <<
                card.getExpirationDate().value() <<
                card.getHolderId().value() <<
                card.getBalance().value() <<
                card.getCategoryId().value();

    card.setPin(1488);
    DBCard::update(card);
    card = DBCard::selectByNumber(5111050540403030);
    qDebug() << card.getNumber().value() <<
                card.getPin().value() <<
                card.getExpirationDate().value() <<
                card.getHolderId().value() <<
                card.getBalance().value() <<
                card.getCategoryId().value();
    DBCard::remove(5111050540403030);
    DBCard::remove(20);
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NavigationWindow w;
    w.show();

    try {
        dbtest();
    } catch (const DatabaseException& ex) {
        qDebug() << "ERROR: " << ex.what();
    }
    transactionTest();
    return a.exec();
//    return 0;
}
