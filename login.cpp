#include "login.h"
#include "ui_login.h"

#include <QDebug>

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
}

void Login::on_confirmLogin_clicked() {
    //this->close();
}

