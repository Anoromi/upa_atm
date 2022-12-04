#include "withdrawmoney.h"
#include "ui_withdrawmoney.h"

WithdrawMoney::WithdrawMoney(std::function<void()> end, QWidget *parent) :
    QWidget(parent),
    _end(end),
    ui(new Ui::WithdrawMoney)
{
    ui->setupUi(this);
}

WithdrawMoney::~WithdrawMoney()
{
    delete ui;
}
