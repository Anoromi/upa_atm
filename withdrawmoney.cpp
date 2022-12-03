#include "withdrawmoney.h"
#include "ui_withdrawmoney.h"

WithdrawMoney::WithdrawMoney(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WithdrawMoney)
{
    ui->setupUi(this);
}

WithdrawMoney::~WithdrawMoney()
{
    delete ui;
}
