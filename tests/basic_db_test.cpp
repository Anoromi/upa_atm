//
// Created by Andrii on 12/3/2022.
//

#include "basic_db_test.h"
#include "middleware/deposit.h"
#include "middleware/signedConnection.h"
#include "middleware/unsignedConnection.h"
#include "backend/database/db_card.h"
#include "backend/bankProvider.h"
#include <string>

using namespace std;

/*
 * returns error name if it occures
 * or string in format 'a b'
 * a - leftover on sender card
 * b - leftover on receiver card
 */
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
                 << details.getTariff()->getDescription(amount)
                 << details.getRecipientCard().getCardNumber() << details.getMoney()
                 << details.getRecipientName() << details.getTariff()->getFee(amount);
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

/*
 * returns error name if it occures
 * or lefover on card after deposit
 */
string testDeposit(const Credentials &to,
                   uint amount,
                   bool isTarrifed) {
    try {
        auto connection = UnsignedConnection().createConnection(to);
        qDebug() << "Before deposit: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        auto request = DepositRequest(amount);
        auto details = connection->getDepositDetails(request);
        qDebug() << "Details:"
                 << details.getTariff().getDescription(amount)
                 << details.getMoney()
                 << details.getTariff().getFee(amount);
        connection->depositMoney(request);
        qDebug() << "After deposit: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        connection->getCardInfo();
        auto res = to_string(connection->getCardInfo().getBalance());
        qDebug() << "result:" << res.c_str();
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

/*
 * returns error name if it occures
 * or lefover on card after withrdrawal
 */
string testWithdraw(const Credentials &from,
                    uint amount,
                    bool isTarrifed) {
    try {
        auto connection = UnsignedConnection().createConnection(from);
        qDebug() << "Before deposit: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        auto request = WithdrawalRequest(amount, false);
        auto details = connection->getWithdrawalDetails(request);
        qDebug() << "Details:"
                 << details.getTariff()->getDescription(amount)
                 << details.getMoney()
                 << details.getTariff()->getFee(amount);
        connection->withdrawMoney(request);
        qDebug() << "After deposit: "
                 << connection->getCardInfo().getBalance()
                 << connection.get()->getCardInfo().getName();
        auto res = to_string(connection->getCardInfo().getBalance());
        qDebug() << "result:" << res.c_str();
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

void restoreTestData(const QSqlDatabase &db) {
    clearDatabase(db);
    createDatabaseSchema(db);
    populateDatabase(db);
}

void checkTest(const char *name, bool (*condition)()) {
    qDebug() << name << "=====================================";
    if (condition()) {
        qDebug() << "PASSED";
    } else {
        qDebug() << "FAILED";
    }
    qDebug() << "============================================";
}

void transactiontests() {
    // tried to make this work on test.db
    // but BankProvider thus Bank uses bank.db anyway
    // so we need to turn tests off for know if we want to work
    // with real data
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    checkTest("trans1",
              [] { return "Bad credentials" == testTransaction({123, 123}, {123}, 0, false); });
    checkTest("trans2",
              [] { return "Bad credentials" == testTransaction({1234567891011121, 123}, {123}, 0, false); });
    checkTest("trans3",
              [] { return "Bad recipient" == testTransaction({1234567891011121, 1234}, {123}, 0, false); });
    checkTest("trans4",
              [] { return "Bad money" == testTransaction({1234567891011121, 1234}, {5168123412341234}, 1000, false); });
    checkTest("trans5",
              [] { return "0 150" == testTransaction({1234567891011121, 1234}, {5168123412341234}, 100, false); });
    restoreTestData(db);
    checkTest("trans6",
              [] { return "0 150" == testTransaction({1234567891011121, 1234}, {5168123412341234}, 100, true); });
    restoreTestData(db);
    checkTest("trans7",
              [] { return "0 150" == testTransaction({5168123412341234, 3221}, {1234567891011121}, 50, false); });
    restoreTestData(db);
    checkTest("trans8",
              [] { return "2 148" == testTransaction({5168123412341234, 3221}, {1234567891011121}, 48, false); });
    restoreTestData(db);
    checkTest("trans9",
              [] { return "0 148" == testTransaction({5168123412341234, 3221}, {1234567891011121}, 48, true); });
}

void deposittests() {
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    checkTest("deposit1",
              [] { return "Bad credentials" == testDeposit({123, 123}, 0, false); });
    checkTest("deposit2",
              [] { return "Bad credentials" == testDeposit({1234567891011121, 123}, 0, false); });
    checkTest("deposit3",
              [] { return "150" == testDeposit({1234567891011121, 1234}, 50, false); });
    auto result = DBTransaction::selectAllById(1234567891011121);
    qDebug() << result.size();
    restoreTestData(db);
    checkTest("deposit4",
              [] { return "140" == testDeposit({1234567891011121, 1234}, 50, true); });
    restoreTestData(db);
    checkTest("deposit5",
              [] { return "100" == testDeposit({1234567891011121, 1234}, 5, true); });
}

void withdrawtests() {
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    checkTest("withdraw1",
              [] { return "Bad credentials" == testWithdraw({123, 123}, 0, false); });
    checkTest("withdraw2",
              [] { return "Bad credentials" == testWithdraw({1234567891011121, 123}, 0, false); });
    checkTest("withdraw3",
              [] { return "0" == testWithdraw({1234567891011121, 1234}, 100, false); });
    restoreTestData(db);
    checkTest("withdraw4",
              [] { return "0" == testWithdraw({1234567891011121, 1234}, 100, true); });
    restoreTestData(db);
    checkTest("withdraw5",
              [] { return "0" == testWithdraw({5168123412341234, 3221}, 50, false); });
    checkTest("withdraw6",
              [] { return "Bad money" == testWithdraw({5168123412341234, 3221}, 50, true); });
}

void top_up_test() {
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    Credentials c = {1234567891011121, 1234};
    qDebug() << "Balance before top up: " <<
             BankProvider::getBank().authorizedCall(c, Bank::getCardInfo).getBalance();
    TopUpRequest req = {10, L"+380501234567"};
    BankProvider::getBank().authorizedCall(c, Bank::performTopUp, req);
    Vector<Transaction> transactions =
            BankProvider::getBank().authorizedCall(c, Bank::getTransactions);
    for (auto &trans: transactions) {
        Optional<Card> sender = trans.getSender();
        Optional<Card> receiver = trans.getReceiver();

        if (sender.has_value()) {
            qDebug() << sender.value().getCardNumber();
        } else {
            qDebug() << "NULL";
        }

        if (receiver.has_value()) {
            qDebug() << receiver.value().getCardNumber();
        } else {
            qDebug() << "NULL";
        }
        qDebug() << trans.getMoney()
                 << trans.getTariff()
                 << trans.getTime()
                 << trans.getDescription();
    }
    qDebug() << "Balance after top up: " <<
             BankProvider::getBank().authorizedCall(c, Bank::getCardInfo).getBalance();
}

void basic_db_test() {
    transactiontests();
    deposittests();
    withdrawtests();
    top_up_test();
}
