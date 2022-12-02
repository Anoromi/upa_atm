#include "success_screen.h"
#include "ui_success_screen.h"

success_screen::success_screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::success_screen)
{
    ui->setupUi(this);
}

success_screen::~success_screen()
{
    delete ui;
}
