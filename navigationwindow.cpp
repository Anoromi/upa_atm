#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include "login.h"

NavigationWindow::NavigationWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::NavigationWindow) {
    ui->setupUi(this);
    ui->navigationStack->addWidget(new Login(UnsignedConnection(), [this](SignedConnection s) {

//        ui->navigationStack->insertWidget(0, );

    }));
    ui->navigationStack->setCurrentIndex(0);
}

NavigationWindow::~NavigationWindow() {
    delete ui;
}

void NavigationWindow::popNav() {
    ui->navigationStack->removeWidget(ui->navigationStack->currentWidget());
}

void NavigationWindow::pushNav(QWidget *w) {
    ui->navigationStack->insertWidget(0, w);
}
