#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include "login.h"
#include "actionsscreen.h"

NavigationWindow::NavigationWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::NavigationWindow) {
    ui->setupUi(this);
    ui->navigationStack->addWidget(new Login(UnsignedConnection(), [this](auto s) {
        this->pushActionsScreen(s);
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
    ui->navigationStack->setCurrentIndex(0);
}

void NavigationWindow::pushActionsScreen(Shared<SignedConnection> s) {
    pushNav(new ActionsScreen(s, [this](auto p) { pushNav(p); }, [this]() { popNav(); }));
}
