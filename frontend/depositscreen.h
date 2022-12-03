#ifndef DEPOSITSCREEN_H
#define DEPOSITSCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"

namespace Ui {
    class DepositScreen;
}

class DepositScreen : public QWidget {
Q_OBJECT

public:
    explicit DepositScreen(SignedConnection &connection, std::function<void()> back,
                           std::function<void(DepositRequest, DepositDetails)> toDetails,
                           QWidget *parent = nullptr);

    ~DepositScreen();

private slots:

    void on_moveNext_clicked();

    void on_back_clicked();

private:
    Ui::DepositScreen *ui;
    SignedConnection &_connection;
    std::function<void()> _back;
    std::function<void(DepositRequest, DepositDetails)> _toDetails;
};

#endif // DEPOSITSCREEN_H
