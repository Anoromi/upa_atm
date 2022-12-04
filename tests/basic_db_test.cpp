//
// Created by Andrii on 12/3/2022.
//

#include "basic_db_test.h"
#include "middleware/deposit.h"
#include "middleware/signedConnection.h"
#include "middleware/unsignedConnection.h"

void test1() {
    try {
        auto connection = UnsignedConnection().createConnection(
                Credentials(Card(1234567891011121), Pin(1234)));

        qDebug() << connection->getCardInfo().getBalance() << connection.get()->getCardInfo().getName();
        auto request = TransferRequest(Card(5168123412341234), 100000, false);
        auto details = connection->getTransferDetails(request);
        qDebug() << details.getRecipientCard().getCardNumber() << ',' << details.getMoney() << ','
                 << details.getRecipientName() << details.getTariff().getFee(100);
        connection->transferMoney(request);
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

        qDebug() << connection->getCardInfo().getBalance() << connection.get()->getCardInfo().getName();
        auto request = WithdrawalRequest(connection->credentials(), 10000, false);
        auto details = connection->getWithdrawalDetails(request);
        qDebug() << details.getMoney() << details.getTariff()->getTariff(10000);
        connection->withdrawMoney(request);
        qDebug() << connection->getCardInfo().getBalance() << connection.get()->getCardInfo().getName();
    }
    catch (BadMoney &) {
        qDebug() << "Bad money works";
    }
    catch (BadRecipient &) {
        qDebug() << "Bad recipient works";
    }

}

void basic_db_test() {
    test1();
//    test2();
//    test3();
}
