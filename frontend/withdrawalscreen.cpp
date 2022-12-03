#include <optional>
#include "withdrawalscreen.h"
#include "ui_withdrawalscreen.h"
#include "parseOrError.h"
#include "middleware/converters.h"
#include "error_message.h"

WithdrawalScreen::WithdrawalScreen(SignedConnection &connection, std::function<void()> back,
                                   std::function<void(WithdrawalRequest, WithdrawalDetails)> toDetails, QWidget *parent)
        :
        QWidget(parent),
        ui(new Ui::WithdrawalScreen),
        _connection(connection), _toDetails(std::move(toDetails)), _back(std::move(back)) {
    ui->setupUi(this);
}

WithdrawalScreen::~WithdrawalScreen() {
    delete ui;
}


void WithdrawalScreen::on_moveNext_clicked() {

    auto moneyRes = parseMoney(ui->money->text().toStdWString());
    if (errorOnString(moneyRes))
        return;
    uint money = std::get<uint>(moneyRes);
    bool withTariff = ui->tariff->isChecked();
    try {
        WithdrawalRequest request(money, withTariff);
        _toDetails(request, _connection.getWithdrawalDetails(request));
    }
    catch (UnexpectedException e) {

    }

}


void WithdrawalScreen::on_back_clicked() {
    _back();
}

