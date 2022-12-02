#include "login.h"
#include "ui_login.h"
#include "middleware/converters.h"

#include <QDebug>
#include <QMessageBox>

Login::Login(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::Login) {
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
}


void Login::on_confirmLogin_clicked() {
}

