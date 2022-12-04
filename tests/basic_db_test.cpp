//
// Created by Andrii on 12/3/2022.
//

#include "basic_db_test.h"
#include "middleware/deposit.h"
#include "middleware/signedConnection.h"
#include "middleware/unsignedConnection.h"
#include "backend/database/db_card.h"
#include <string>

using namespace std;

string testTransaction(const Credentials &from,
                           const Card &to,
                           uint amount,
                           bool isTarrifed) {
    try {
        auto connection = UnsignedConnection().createConnection(from);
        qDebug() << "Before transfer: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        auto request = TransferRequest(to.getCardNumber(), amount, isTarrifed);
        auto details = connection->getTransferDetails(request);
        qDebug() << "Details: "
                 << details.getRecipientCard().getCardNumber() << details.getMoney()
                 << details.getRecipientName() << details.getTariff().getFee(amount);
        connection->transferMoney(request);
        qDebug() << "After transfer: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        connection->getCardInfo();
        auto res = to_string(DBCard::selectByNumber(from.card().getCardNumber()).getBalance().value())
               + " "
               + to_string(DBCard::selectByNumber(to.getCardNumber()).getBalance().value());
        qDebug() << res.c_str();
        return res;
    }
    catch (BadMoney &) {
        return "Bad money";
    }
    catch (BadRecipient &) {
        return "Bad recipient";
    }
    catch (UnexpectedException &) {
        return "Bad credentials";
    }
}

void test1_1() {
    try {
        auto connection = UnsignedConnection().createConnection(
                Credentials(Card(5168123412341234), Pin(3221)));
        qDebug() << "Before transfer: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        auto request = TransferRequest(Card(5168123412341234), 50000, true);
        auto details = connection->getTransferDetails(request);
        qDebug() << "Details: "
                 << details.getRecipientCard().getCardNumber() << details.getMoney()
                 << details.getRecipientName() << details.getTariff().getFee(100);
        connection->transferMoney(request);
        qDebug() << "After transfer: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
    }
    catch (BadMoney &) {
        qDebug() << "Bad money works";
    }
    catch (BadRecipient &) {
        qDebug() << "Bad recipient works";
    }
}

void test2() {
    try {
        auto connection = UnsignedConnection().createConnection(
                Credentials(Card(1234567891011121), Pin(1234)));

        qDebug() << connection->getCardInfo().getBalance() << connection.get()->getCardInfo().getName();
        auto request = DepositRequest(connection->credentials(), 10000, false);
        auto details = connection->getDepositDetails(request);
        qDebug() << details.getMoney() << details.getTariff().getFee(100);
        connection->depositMoney(request);
    }
    catch (BadMoney &) {
        qDebug() << "Bad money works";
    }
    catch (BadRecipient &) {
        qDebug() << "Bad recipient works";
    }
}

void test3() {
    try {
        auto connection = UnsignedConnection().createConnection(
                Credentials(Card(1234567891011121), Pin(1234)));

        qDebug() << "before withdraw: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        auto request = WithdrawalRequest(connection->credentials(), 10000, false);
        auto details = connection->getWithdrawalDetails(request);
        qDebug() << "Details: " <<
                    details.getMoney() <<
                    details.getTariff()->getTariff(10000);
        connection->withdrawMoney(request);
        qDebug() << "After withdraw: " <<
                    connection->getCardInfo().getBalance() <<
                    connection.get()->getCardInfo().getName();
    }
    catch (BadMoney &) {
        qDebug() << "Bad money works";
    }
    catch (BadRecipient &) {
        qDebug() << "Bad recipient works";
    }

}

void restoreTestData(const QSqlDatabase &db) {
    clearDatabase(db);
    createDatabaseSchema(db);
    populateDatabase(db);
}

void checkTest(const char* name, bool (*condition)()) {
    qDebug() << name << "=====================================";
    if (condition()) {
        qDebug() << "PASSED";
    } else {
        qDebug() << "FAILED";
    }
    qDebug() << "============================================";
}

void basic_db_test() {
    // tried to make this work on test.db
    // but BankProvider thus Bank uses bank.db anyway
    // so we need to turn tests off for know if we want to work
    // with real data
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    checkTest("trans1",
              [] {return "Bad credentials" == testTransaction({123,123}, {123}, 0, false);});
    checkTest("trans2",
              [] {return "Bad credentials" == testTransaction({1234567891011121,123}, {123}, 0, false);});
    checkTest("trans3",
              [] {return "Bad recipient" == testTransaction({1234567891011121,1234}, {123}, 0, false);});
    checkTest("trans4",
              [] {return "Bad money" == testTransaction({1234567891011121,1234}, {5168123412341234}, 1000, false);});
    checkTest("trans5",
              [] {return "0 150" == testTransaction({1234567891011121,1234}, {5168123412341234}, 100, false);});
    restoreTestData(db);
    checkTest("trans6",
              [] {return "0 150" == testTransaction({1234567891011121,1234}, {5168123412341234}, 100, true);});
    restoreTestData(db);
    checkTest("trans7",
              [] {return "50 100" == testTransaction({5168123412341234,3221}, {1234567891011121}, 50, false);});
    restoreTestData(db);
    checkTest("trans8",
              [] {return "0 150" == testTransaction({5168123412341234,3221}, {1234567891011121}, 50, false);});
    restoreTestData(db);
    checkTest("trans9",
              [] {return "2 148" == testTransaction({5168123412341234,3221}, {1234567891011121}, 48, false);});
    restoreTestData(db);
    checkTest("trans10",
              [] {return "0 150" == testTransaction({5168123412341234,3221}, {1234567891011121}, 48, true);});

    restoreTestData(db);
}
