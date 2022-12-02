#include "login.h"
#include "ui_login.h"
#include "middleware/converters.h"
#include "frontend/error_message.h"

#include <QDebug>
#include <QMessageBox>

Login::Login(UnsignedConnection uConnection,
             std::function<void(SignedConnection)> enter, QWidget *parent)
        : QMainWindow(parent), ui(new Ui::Login), _uConnection(uConnection), _enter(std::move(enter)) {
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
}


void Login::on_confirmLogin_clicked() {
    auto cardRes = parseCard(ui->cardNumberField->text().toStdWString());
    if (cardRes.index() == 1) {
        errorMessage(std::get<String>(cardRes));
        return;
    }
//    auto pinNumber =
//    if(_uConnection.validateCredentials())
//    _enter();
}

