//
// Created by Andrii on 12/3/2022.
//

#include "basic_db_test.h"
#include "backend/bank.h"
#include "middleware/signedConnection.h"
#include "middleware/unsignedConnection.h"

void basic_db_test() {
    try {
        auto connection = UnsignedConnection().createConnection(
                Credentials(Card(1234567891011121), Pin(1234)));

        qDebug() << connection->getCardInfo().getBalance() << connection.get()->getCardInfo().getName();
        auto request = TransferRequest(Card(5168123412341234), 100, false);
        auto details = connection->getTransferDetails(request);
        qDebug() << details.getRecipientCard().getCardNumber() << ',' << details.getMoney() << ','
                 << details.getRecipientName() << details.getTariff().getFee(100);
        connection->transferMoney(request);
    }
    catch(...) {

    }
}
