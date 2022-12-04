#include "actionsscreen.h"
#include "transaction_details.h"
#include "transaction_screen.h"
#include "ui_actionsscreen.h"

#include "frontend/withdrawalscreen.h"
#include "frontend/put_money.h"
#include "frontend/phonewindow.h"

ActionsScreen::ActionsScreen(Shared<SignedConnection> &s, std::function<void(QWidget *destination)> push,
                             std::function<void()> pop, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActionsScreen),
    _connect(s),
    _push(std::move(push)),
    _pop(std::move(pop))
{
    ui->setupUi(this);

    QString buff;

    buff.setNum(s->credentials().card().getCardNumber());
    ui->cardNumber->setText(buff);
    //:TODO: get balance from db
    buff.setNum(0);
    ui->balance->setText(buff);
}

ActionsScreen::~ActionsScreen() {
    delete ui;
}

void ActionsScreen::endSession() {
    _pop();
}

void ActionsScreen::on_transfer_clicked() {
    _push(new TransactionScreen(
              nullptr,
              *this->_connect,
              _push
          )
    );
}


void ActionsScreen::on_withdraw_clicked() {
    _push(new WithdrawalScreen());
}


void ActionsScreen::on_refil_clicked() {
    _push(new put_money());
}


void ActionsScreen::on_refilMobile_clicked()
{
    _push(new PhoneWindow(this));
}

