#include "transaction_screen.h"

#include <utility>
#include "ui_transaction_screen.h"
#include "middleware/converters.h"
#include "frontend/error_message.h"

//transaction_screen::transaction_screen(QWidget *parent) :
//        QWidget(parent),
//        ui(new Ui::transaction_screen) {
//    ui->setupUi(this);
//}

transaction_screen::~transaction_screen() {
    delete ui;
}

void transaction_screen::on_pushButton_clicked() {
    auto cardRes = parseCard(ui->card->text().toStdWString());
    if (cardRes.index() == 1) {
        errorMessage(std::get<String>(cardRes));
        return;
    }
    auto moneyRes = parseMoney(ui->money->text().toStdWString());
    if (moneyRes.index() == 1) {
        errorMessage(std::get<String>(moneyRes));
        return;
    }
    Card c = std::get<Card>(cardRes);
    uint money = std::get<uint>(moneyRes);
    bool afterTariff = ui->withTariff->isChecked();
    try {
        TransferRequest request = TransferRequest(c, money, afterTariff);
        TransferDetails details = _connection.getTransferDetails(request);
        _moveToDetails(request, std::move(details));
    }
    catch (const BadMoney &m) {
        errorMessage((std::wstringstream() << L"Ви не можете витратити більше " << moneyToString(m.getAvailable())
                                           << L", а запитали " << moneyToString(m.getRequested())).str());
    }

}

transaction_screen::transaction_screen(QWidget *parent, const SignedConnection &connection,
                                       std::function<void(TransferRequest, TransferDetails)> toDetails)
        : QWidget(parent), _connection(connection), _moveToDetails(std::move(toDetails)), ui(new Ui::transaction_screen) {
    ui->setupUi(this);
}

