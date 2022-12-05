#include "transactionitem.h"
#include "ui_transactionitem.h"

TransactionItem::TransactionItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionItem)
{
    ui->setupUi(this);
}

TransactionItem::~TransactionItem()
{
    delete ui;
}
