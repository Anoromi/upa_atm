#include "withdrawscreen.h"
#include "ui_withdrawscreen.h"

WithdrawScreen::WithdrawScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WithdrawScreen)
{
    ui->setupUi(this);
}

WithdrawScreen::~WithdrawScreen()
{
    delete ui;
}
