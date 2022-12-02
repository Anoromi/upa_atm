//
// Created by Andrii on 12/1/2022.
//

#include "error_message.h"

void errorMessage(const String& message) {
    QMessageBox msg;
    msg.setText(QString::fromWCharArray(message.c_str()));
    msg.exec();
}
