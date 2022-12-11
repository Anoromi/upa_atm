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
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    checkTest("Bad sender card number",
              [] { return "Bad credentials" == testTransaction({123, 123}, {123}, 0, false); });
    checkTest("Bad sender pin",
              [] { return "Bad credentials" == testTransaction({1234567891011121, 123}, {123}, 0, false); });
    checkTest("Bad receiver card",
              [] { return "Bad recipient" == testTransaction({1234567891011121, 1234}, {123}, 0, false); });
    checkTest("Insufficient funds on sender card without fee",
              [] { return "Bad money" == testTransaction({1234567891011121, 1234}, {5168123412341234}, 1000, false); });
    checkTest("Correctly send money without fee on premium card",
              [] { return "0 150" == testTransaction({1234567891011121, 1234}, {5168123412341234}, 100, false); });
    restoreTestData(db);
    checkTest("Correctly apply fee on premium card",
              [] { return "0 150" == testTransaction({1234567891011121, 1234}, {5168123412341234}, 100, true); });
    restoreTestData(db);
    checkTest("Correctly send all money from universal card without fee",
              [] { return "0 150" == testTransaction({5168123412341234, 3221}, {1234567891011121}, 50, false); });
    restoreTestData(db);
    checkTest("Correctly send money from universal card without fee",
              [] { return "2 148" == testTransaction({5168123412341234, 3221}, {1234567891011121}, 48, false); });
    restoreTestData(db);
    checkTest("Correctly send money from universal card with fee",
              [] { return "0 148" == testTransaction({5168123412341234, 3221}, {1234567891011121}, 48, true); });
}

void deposittests() {
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    checkTest("Do not deposit on incorrect card",
              [] { return "Bad credentials" == testDeposit({123, 123}, 0, false); });
    checkTest("Do not deposit on incorrect pin",
              [] { return "Bad credentials" == testDeposit({1234567891011121, 123}, 0, false); });
    checkTest("Correctly add money to account without fee",
              [] { return "150" == testDeposit({1234567891011121, 1234}, 50, false); });
    auto result = DBTransaction::selectAllById(1234567891011121);
    qDebug() << result.size();
    restoreTestData(db);
    checkTest("Correctly add money with fee",
              [] { return "140" == testDeposit({1234567891011121, 1234}, 50, true); });
    restoreTestData(db);
    checkTest("Add no money when amount is less than fee",
              [] { return "100" == testDeposit({1234567891011121, 1234}, 5, true); });
}

void withdrawtests() {
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    restoreTestData(db);
    checkTest("Do not withdraw from incorrect card",
              [] { return "Bad credentials" == testWithdraw({123, 123}, 0, false); });
    checkTest("Do not withdraw on incorrect pin",
              [] { return "Bad credentials" == testWithdraw({1234567891011121, 123}, 0, false); });
    checkTest("Correctly withdraw all money",
              [] { return "0" == testWithdraw({1234567891011121, 1234}, 100, false); });
    restoreTestData(db);
    checkTest("Correctly withdraw all money from universal card",
              [] { return "0" == testWithdraw({5168123412341234, 3221}, 50, false); });
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
