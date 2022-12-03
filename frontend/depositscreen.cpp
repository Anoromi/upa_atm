#include "depositscreen.h"
#include "ui_depositscreen.h"
#include "middleware/converters.h"
#include "parseOrError.h"

DepositScreen::DepositScreen(SignedConnection &connection, std::function<void()> back,
                             std::function<void(DepositRequest, DepositDetails)> toDetails, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DepositScreen),
        _connection(connection),
        _back(std::move(back)),
        _toDetails(std::move(toDetails)) {
    ui->setupUi(this);
}

DepositScreen::~DepositScreen() {
    delete ui;
}

void DepositScreen::on_moveNext_clicked() {
    auto moneyRes = parseMoney(ui->money->text().toStdWString());
    if (errorOnString(moneyRes))
        return;
    uint money = std::get<uint>(moneyRes);
    try {
        DepositRequest request(money);
        _toDetails(request, _connection.getDepositDetails(request));
    } catch (UnexpectedException e) {

    }
}


void DepositScreen::on_back_clicked() {
    _back();
}

