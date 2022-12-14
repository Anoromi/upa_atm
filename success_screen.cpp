#include "success_screen.h"
#include "ui_success_screen.h"

success_screen::success_screen(std::function<void()> end, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::success_screen),
        _end(std::move(end)) {
    ui->setupUi(this);
}

success_screen::~success_screen() {
    delete ui;
}

void success_screen::on_end_button_clicked() {
    _end();
}

