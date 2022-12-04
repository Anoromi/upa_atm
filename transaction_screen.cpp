#include "transaction_screen.h"

#include <utility>
#include "ui_transaction_screen.h"
#include "transaction_details.h"
#include "middleware/converters.h"
#include "frontend/error_message.h"


//transaction_screen::transaction_screen(QWidget *parent) :
//        QWidget(parent),
//        ui(new Ui::transaction_screen) {
//    ui->setupUi(this);
//}

TransactionScreen::TransactionScreen(
        QWidget *parent,
        const SignedConnection &connection,
        std::function<void(TransferRequest, TransferDetails)> toDetails,
        std::function<void()> back
)
        :
        QWidget(parent),
        _connection(connection),
        _toDetails(std::move(toDetails)),
        _back(std::move(back)),
        ui(new Ui::TransactionScreen) {
    ui->setupUi(this);
}

TransactionScreen::~TransactionScreen() {
    delete ui;
}


//void TransactionScreen::toDetails(const TransferRequest &request, const TransferDetails &details) {
//    this->toDetails(
//            new TransactionDetails
//                    (
//                            details.getRecipientName(),
//                            details.getRecipientCard(),
//                            details.getTariff(),
//                            details.getMoney(),
//                            [this, &request](bool b) {
//                                if (b) {
//                                    _mainMenuPush(this);
//                                } else {
//                                    try {
//                                        _connection.transferMoney(request);
//                                    } catch (UnexpectedException &e) {
//                                        showErrorMessage(L"Something went wrong");
//                                    }
//                                    _mainMenuPush(new success_screen([this]() { _mainMenuPush(this); }));
//                                }
//                            }
//                    )
//    );
//}

void TransactionScreen::on_submitButton_clicked() {
    auto cardRes = parseCard(ui->card->text().toStdWString());
    if (cardRes.index() == 1) {
        showErrorMessage(std::get<String>(cardRes));
        return;
    }
    auto moneyRes = parseMoney(ui->money->text().toStdWString());
    if (moneyRes.index() == 1) {
        showErrorMessage(std::get<String>(moneyRes));
        return;
    }
    Card c = std::get<Card>(cardRes);
    uint money = std::get<uint>(moneyRes);
    if (money > _connection.getCardInfo().getBalance()) {
        showErrorMessage((std::wstringstream() << L"Ви не можете витратити більше " << moneyToString(_connection.getCardInfo().getBalance())
                                               << L", а запитали " << moneyToString(money)).str());
        return;
    }
    bool afterTariff = ui->withTariff->isChecked();
    try {
        TransferRequest request = TransferRequest(c, money, afterTariff);
        TransferDetails details = _connection.getTransferDetails(request);
        _toDetails(request, std::move(details));
    }
    catch (const BadMoney &m) {
        showErrorMessage((std::wstringstream() << L"Ви не можете витратити більше " << moneyToString(m.getAvailable())
                                               << L", а запитали " << moneyToString(m.getRequested())).str());
    }
    catch (UnexpectedException &e) {
        showErrorMessage(L"Такого рахунку не існує");
    }
}


void TransactionScreen::on_back_clicked() {
    _back();
}

