#include "actionsscreen.h"
#include "transaction_details.h"
#include "transaction_screen.h"
#include "ui_actionsscreen.h"
#include "middleware/converters.h"

ActionsScreen::ActionsScreen(Shared<SignedConnection> &s, std::function<void(QWidget *destination)> push,
                             std::function<void()> pop, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ActionsScreen),
        _connect(s),
        _push(std::move(push)),
        _pop(std::move(pop)) {
    ui->setupUi(this);

    QString buff;

    buff.setNum(s->credentials().card().getCardNumber());
    ui->cardNumber->setText(buff);
    //:TODO: get balance from db
//    buff.setNum(_connect.get()->getCardInfo().getBalance());

    ui->balance->setText(stringToQ(moneyToString(_connect.get()->getCardInfo().getBalance())));
}

ActionsScreen::~ActionsScreen() {
    delete ui;
}

void ActionsScreen::endSession() {
    _pop();
}

void ActionsScreen::on_transfer_clicked() {
//    _push(new TransactionScreen(
//              *this->_connect,
//              _push,
//              nullptr,
//              nullptr
//          )
//    );
}

