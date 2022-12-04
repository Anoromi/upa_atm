#include "withdrawalscreen.h"
#include "frontend/error_message.h"
#include "middleware/converters.h"
#include "ui_withdrawalscreen.h"

WithdrawalScreen::WithdrawalScreen(
        std::function<void(const WithdrawalRequest&, const WithdrawalDetails&)> proceed,
        std::function<void()> back,
        const SignedConnection& connection,
        QWidget *parent
        ) :
    QWidget(parent),
    _proceed(proceed),
    _back(back),
    _connection(connection),
    ui(new Ui::WithdrawalScreen)
{
    ui->setupUi(this);
}

WithdrawalScreen::~WithdrawalScreen() {
    delete ui;
}

void WithdrawalScreen::on_confirm_clicked() {
    std::variant<uint, String> value = parseMoney(this->ui->money->text().toStdWString());
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
        WithdrawalRequest request(_connection.credentials(), withdrawAmount, false);
        WithdrawalDetails details(_connection.getWithdrawalDetails(request));
        _proceed(request, details);
    } catch (UnexpectedException& e) {
        showErrorMessage(e.message());
        return;
    }
}


void WithdrawalScreen::on_back_clicked() {
    _back();
}

