#include "phonewindow.h"
#include "ui_phonewindow.h"

PhoneWindow::PhoneWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhoneWindow)
{
    ui->setupUi(this);
}

PhoneWindow::~PhoneWindow()
{
    delete ui;
}

void PhoneWindow::on_confirm_clicked() {

}


void PhoneWindow::on_cancel_clicked()
{

}

