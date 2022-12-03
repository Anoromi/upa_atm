#include "transaction_details.h"
#include "ui_transaction_details.h"
#include "middleware/converters.h"

transaction_details::transaction_details(
        String message,
        std::optional<Card> receiver,
        const Tariff& tariff,
        uint money,
        std::function<void(bool)> move,
        QWidget *parent
) :

        QWidget(parent),
        ui(new Ui::transaction_details),
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
        ui->receiver_card->setText(stringToQ((std::wostringstream() << _receiver.value().getCardNumber()).str()));
    }
    ui->money->setText(stringToQ(moneyToString(money)));
    ui->message->setText(stringToQ(message));
    ui->tariff_definition->setText(stringToQ(message));
    ui->tariff_cost->setText(stringToQ(message));
    ui->gridLayout->update();
}

transaction_details::~transaction_details() {
    delete ui;
}

void transaction_details::on_continue_2_clicked()
{
    _navigateNext(true);
}


void transaction_details::on_cancel_clicked()
{
    _navigateNext(false);
}

