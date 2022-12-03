#include "withdrawalscreen.h"
#include "ui_withdrawalscreen.h"

WithdrawalScreen::WithdrawalScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WithdrawalScreen)
{
    ui->setupUi(this);
}

WithdrawalScreen::~WithdrawalScreen() {
    delete ui;
}
