#include "transactionitem.h"
#include "ui_transactionitem.h"
#include "middleware/converters.h"

TransactionItem::TransactionItem(const Transaction &transaction, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::TransactionItem),
        _transaction(transaction) {
    ui->setupUi(this);
    ui->money;
    if (_transaction.getSender().has_value() && _transaction.getReceiver().has_value()) {
        ui->transactionName->setText(stringToQ(L"Транзакція"));
    } else if (_transaction.getSender().has_value()) {
        ui->transactionName->setText(stringToQ(L"Знімання"));
    } else {
        ui->transactionName->setText(stringToQ(L"Депозит"));
    }
    if (_transaction.getSender().has_value()) {
        ui->sender->setText(stringToQ(ullongToString(_transaction.getSender().value().getCardNumber())));
    } else {
        ui->sender->setText("");
        ui->sender_info->setText("");
    }
    if (_transaction.getReceiver()) {
        ui->sender->setText(stringToQ(ullongToString(_transaction.getReceiver().value().getCardNumber())));
    } else {
        ui->receiver->setText("");
        ui->receiver_info->setText("");
    }
    ui->money->setText(stringToQ(moneyToString(_transaction.getMoney())));
    ui->tariff->setText(stringToQ(moneyToString(_transaction.getTariff())));
}

TransactionItem::~TransactionItem() {
    delete ui;
}
