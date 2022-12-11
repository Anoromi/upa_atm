#include "transaction_details.h"
#include "ui_transaction_details.h"
#include "middleware/converters.h"

TransactionDetails::TransactionDetails(
        String message,
        std::optional<Card> receiver,
        const Shared<Tariff> tariff,
        uint money,
        std::function<void(bool)> move,
        QWidget *parent
) :

        QWidget(parent),
        ui(new Ui::TransactionDetails),
        _message(std::move(message)),
        _receiver(receiver),
        _tariff(tariff),
        _money(money),
        _navigateNext(std::move(move)) {
    ui->setupUi(this);
    if (!_receiver.has_value()) {
        ui->gridLayout->removeWidget(ui->receiver_label);
        ui->gridLayout->removeWidget(ui->receiver_card);
    } else {
        ui->receiver_card->setText(stringToQ(ullongToString(_receiver->getCardNumber())));
    }
    ui->money->setText(stringToQ(moneyToString(money)));
    ui->message->setText(stringToQ(message));
    ui->tariff_definition->setText(stringToQ(_tariff->getDescription(money)));
    ui->tariff_cost->setText(stringToQ(moneyToString(_tariff->getFee(money))));
    ui->gridLayout->update();
}

TransactionDetails::~TransactionDetails() {
    delete ui;
}

void TransactionDetails::on_continue_2_clicked() {
    _navigateNext(true);
}


void TransactionDetails::on_cancel_clicked() {
    _navigateNext(false);
}

