#include "actionsscreen.h"
#include "ui_actionsscreen.h"

ActionsScreen::ActionsScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActionsScreen)
{
    ui->setupUi(this);
}

ActionsScreen::~ActionsScreen()
{
    delete ui;
}
