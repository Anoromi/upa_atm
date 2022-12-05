#include "transactionhistoryscreen.h"
#include "ui_transactionhistoryscreen.h"

TransactionHistoryScreen::TransactionHistoryScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionHistoryScreen)
{
    ui->setupUi(this);
}

TransactionHistoryScreen::~TransactionHistoryScreen()
{
    delete ui;
}
