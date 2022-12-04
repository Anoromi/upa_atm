//
// Created by Andrii on 12/1/2022.
//

#include "error_message.h"

void showErrorMessage(const String& message) {
    QMessageBox msg;
    msg.setText(QString::fromWCharArray(message.c_str()));
    msg.exec();
}
