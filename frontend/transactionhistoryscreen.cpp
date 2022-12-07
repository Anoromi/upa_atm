#include <QStandardItemModel>
#include "transactionhistoryscreen.h"
#include "qnamespace.h"
#include "ui_transactionhistoryscreen.h"
#include "middleware/transaction.h"
#include "middleware/signedConnection.h"
#include "transactionitem.h"
#include "middleware/converters.h"

TransactionHistoryScreen::TransactionHistoryScreen(SignedConnection &connection, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TransactionHistoryScreen),
        _connection(connection) {
    ui->setupUi(this);
    Vector<Transaction> transactions = _connection.getTransactions();
    QStandardItemModel *model = new QStandardItemModel(5, transactions.size());
    ui->tableView->setModel(model);

    for (auto &t: transactions) {
        QList<QStandardItem *> row;
        if (t.getReceiver().has_value() && t.getSender().has_value()) {
            row.append(new QStandardItem(QString::fromWCharArray(L"Транзакція")));
        } else if (t.getReceiver().has_value())
            row.append(new QStandardItem(QString::fromWCharArray(L"Депозит")));
        else
            row.append(new QStandardItem(QString::fromWCharArray(L"Знімання")));
        if (t.getSender().has_value())
            row.append(new QStandardItem(
                    stringToQ(ullongToString(t.getSender().value().getCardNumber()))));
        else row.append(new QStandardItem(""));


        if (t.getReceiver().has_value())
            row.append(new QStandardItem(
                    stringToQ(ullongToString(t.getReceiver().value().getCardNumber()))));
        else row.append(new QStandardItem(""));

        row.append(new QStandardItem(stringToQ(moneyToString(t.getMoney()))));
        row.append(new QStandardItem(stringToQ(moneyToString(t.getTariff()))));
        model->appendRow(row);
    }

}

TransactionHistoryScreen::~TransactionHistoryScreen() {
    delete ui;
}

void TransactionHistoryScreen::on_pushButton_clicked() {

}

