#include "depositscreen.h"
#include "ui_depositscreen.h"

DepositScreen::DepositScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepositScreen)
{
    ui->setupUi(this);
}

DepositScreen::~DepositScreen()
{
    delete ui;
}
