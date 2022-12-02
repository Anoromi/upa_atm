#include "actionsscreen.h"
#include "ui_actionsscreen.h"

ActionsScreen::ActionsScreen(Shared<SignedConnection> &s, std::function<void(QWidget *destination)> push,
                             std::function<void()> pop, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ActionsScreen),
        s(s),
        _push(std::move(push)),
        _pop(std::move(pop)) {
    ui->setupUi(this);
}

ActionsScreen::~ActionsScreen() {
    delete ui;
}

void ActionsScreen::endSession() {
    _pop();
}
