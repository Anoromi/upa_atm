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
#include "backend/database/db_util.h"
#include "tests/basic_db_test.h"

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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NavigationWindow w;
    w.show();
    // remove true to save data
    initDatabase("bank.db", true); // todo move it somewhere else?
    basic_db_test();
    return a.exec();
}
