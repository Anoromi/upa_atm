#include "actionsscreen.h"
#include "ui_actionsscreen.h"

ActionsScreen::ActionsScreen(Shared<SignedConnection>& s, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ActionsScreen),
        s(s) {
    ui->setupUi(this);
}

ActionsScreen::~ActionsScreen() {
    delete ui;
}
