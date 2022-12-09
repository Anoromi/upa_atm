#include "phonewindow.h"
#include "ui_phonewindow.h"

#include "middleware/converters.h"
#include "error_message.h"

PhoneWindow::PhoneWindow(
        std::function<void(const TopUpRequest &)> proceed,
        std::function<void()> back,
        const SignedConnection &connetion,
        QWidget *parent
) :
        _proceed(proceed),
        _back(back),
        _connection(connetion),
        QWidget(parent),
        ui(new Ui::PhoneWindow) {
    ui->setupUi(this);
}

PhoneWindow::~PhoneWindow() {
    delete ui;
}

void PhoneWindow::on_confirm_clicked() {
    std::variant<uint, String> value = parseMoney(this->ui->amountEdit->text().toStdWString());
    if (value.index() == 1) {
        showErrorMessage(std::get<String>(value));
        return;
    }
    uint withdrawAmount = std::get<uint>(value);
    if (_connection.getCardInfo().getBalance() < withdrawAmount) {
        showErrorMessage(L"You can't get that amount of money from your card");
        return;
    }
    try {
        TopUpRequest request(withdrawAmount, this->ui->phoneEdit->text().toStdWString());
        _proceed(request);
    }
    catch (UnexpectedException &e) {
        showErrorMessage(e.message());
        return;
    }
}


void PhoneWindow::on_cancel_clicked() {
    _back();
}

