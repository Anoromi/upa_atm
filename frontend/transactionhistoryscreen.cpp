#include <QStandardItemModel>
#include "transactionhistoryscreen.h"
#include "qnamespace.h"
#include "ui_transactionhistoryscreen.h"
#include "middleware/transaction.h"
#include "middleware/signedConnection.h"
#include "middleware/converters.h"

TransactionHistoryScreen::TransactionHistoryScreen(SignedConnection &connection, std::function<void()> back,
                                                   QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TransactionHistoryScreen),
        _back(std::move(back)),
        _connection(connection) {
    ui->setupUi(this);
    Vector<Transaction> transactions = _connection.getTransactions();
    QStandardItemModel *model = new QStandardItemModel(0, 5);
    ui->tableView->setModel(model);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString::fromWCharArray(L"Тип")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString::fromWCharArray(L"Надсилач")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString::fromWCharArray(L"Отримувач")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString::fromWCharArray(L"Сума")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString::fromWCharArray(L"Тариф")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString::fromWCharArray(L"Опис")));


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
        row.append(new QStandardItem(stringToQ(t.getDescription())));
        model->appendRow(row);
    }

    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

TransactionHistoryScreen::~TransactionHistoryScreen() {
    delete ui;
}

void TransactionHistoryScreen::on_pushButton_clicked() {
    _back();
}

