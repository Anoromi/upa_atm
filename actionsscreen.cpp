#include "actionsscreen.h"
#include "success_screen.h"

#include <utility>
#include "frontend/error_message.h"
#include "transaction_details.h"
#include "transaction_screen.h"
#include "ui_actionsscreen.h"

#include "frontend/withdrawalscreen.h"
#include "frontend/put_money.h"
#include "frontend/phonewindow.h"
#include "middleware/converters.h"

void ActionsScreen::updateCardInfo() {
    CardInfo info = _connect->getCardInfo();
    QString buff;
    buff.setNum(_connect->credentials().card().getCardNumber());
    ui->cardNumber->setText(buff);
    ui->balance->setText(stringToQ(moneyToString(info.getBalance())));
}

ActionsScreen::ActionsScreen(Shared<SignedConnection> &s, std::function<void(QWidget *destination)> push,
                             std::function<void()> pop, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ActionsScreen),
        _connect(s),
        _push(std::move(push)),
        _pop(std::move(pop)) {
    ui->setupUi(this);

    updateCardInfo();
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
                  [this](auto request, auto details) {
                      toDetails(L"Перевести гроші", request.getDestination(), details.getTariff(), details.getMoney(),
                                [this, request]() {
                                    try {
                                        this->_connect->transferMoney(request);
                                    } catch (UnexpectedException &e) {
                                        showErrorMessage(e.message());
                                        return;
                                    }
                                    _push(new success_screen([this]() {
                                        _pop();
                                        _pop();
                                        _pop();
                                        updateCardInfo();

                                    }));
                                });
                  },
                  _pop
          )
    );
}

void ActionsScreen::toDetails(
        String message,
        std::optional<Card> receiver,
        Shared<Tariff> tariff,
        uint money,
        std::function<void()> performAction
) {
    _push(new TransactionDetails(std::move(message), receiver, tariff, money,
                                 [this, performAction = std::move(performAction)](bool v) {
                                     if (v)
                                         performAction();
                                     else
                                         _pop();
                                 }));
}


void ActionsScreen::on_withdraw_clicked() {
    _push(
            new WithdrawalScreen(
                    [this](const WithdrawalRequest &request, const WithdrawalDetails &details) {
                        try {
                            this->_connect->withdrawMoney(request);
                        } catch (UnexpectedException &e) {
                            showErrorMessage(e.message());
                            return;
                        }
                        _push(
                                new success_screen(
                                        [this]() { this->_push(this); }
                                )
                        );
                    },
                    [this]() { this->_pop(); },
                    *this->_connect
            )
    );
}


void ActionsScreen::on_refil_clicked() {
    _push(new put_money());
}


void ActionsScreen::on_refilMobile_clicked() {
    _push(new PhoneWindow(this));
}

