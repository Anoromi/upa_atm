#include "login.h"
#include "ui_login.h"
#include "middleware/converters.h"
#include "frontend/error_message.h"

#include <QDebug>
#include <QMessageBox>

Login::Login(UnsignedConnection uConnection,
             std::function<void(Shared<SignedConnection>)> enter, QWidget *parent)
        : QMainWindow(parent), ui(new Ui::Login), _uConnection(uConnection), _enter(std::move(enter)) {
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
}


void Login::on_confirmLogin_clicked() {
    auto cardRes = parseCard(ui->cardNumberField->text().toStdWString());
    qDebug() << cardRes.index() << ',' ;
    if (cardRes.index() == 1) {
        errorMessage(std::get<String>(cardRes));
        return;
    }
    auto pinRes = parsePin(ui->passwordField->text().toStdWString());
    qDebug() << pinRes.index();
    if (pinRes.index() == 1) {
        errorMessage(std::get<String>(pinRes));
        return;
    }
    Card card = std::get<Card>(cardRes);
    Pin pin = std::get<Pin>(pinRes);
    try {
        _enter(_uConnection.createConnection({card, pin}));
    }
    catch (BlockedCard) {
        errorMessage(L"Ваша карта заблокована");
    }
}

