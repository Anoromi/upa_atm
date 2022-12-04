#include "put_money.h"
#include "ui_put_money.h"
#include "frontend/error_message.h"
#include "middleware/converters.h"

PutMoney::PutMoney
        (
                std::function<void(const DepositRequest &, const DepositDetails &)> proceed,
                std::function<void()> back,
                const SignedConnection &connection,
                QWidget *parent
        ) :
        _proceed(proceed),
        _back(back),
        _connection(connection),
        QWidget(parent),
        ui(new Ui::PutMoney) {
    ui->setupUi(this);
}

PutMoney::~PutMoney() {
    delete ui;
}

void PutMoney::on_put_clicked() {
    std::variant<uint, String> readMoney(parseMoney(this->ui->moneyLine->text().toStdWString()));
    if (readMoney.index() == 1) {
        showErrorMessage(std::get<String>(readMoney));
        return;
    }
    uint money = std::get<uint>(readMoney);
    try {
        DepositRequest request(_connection.credentials(), money, false);
        DepositDetails details(_connection.getDepositDetails(request));
        _proceed(request, details);
    }
    catch (UnexpectedException &e) {
        showErrorMessage(e.message());
        return;
    }
}

void PutMoney::on_back_clicked() {
    _back();
}

