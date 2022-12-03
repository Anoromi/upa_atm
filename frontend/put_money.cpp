#include "put_money.h"
#include "ui_put_money.h"

put_money::put_money(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::put_money)
{
    ui->setupUi(this);
}

put_money::~put_money()
{
    delete ui;
}
