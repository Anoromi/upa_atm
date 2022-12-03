#ifndef WITHDRAWALSCREEN_H
#define WITHDRAWALSCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"

namespace Ui {
    class WithdrawalScreen;
}

class WithdrawalScreen : public QWidget {
Q_OBJECT

public:
    explicit WithdrawalScreen(SignedConnection &connection, std::function<void()> back,
                              std::function<void(WithdrawalRequest, WithdrawalDetails)> toDetails,
                              QWidget *parent = nullptr);

    ~WithdrawalScreen();

private slots:

    void on_moveNext_clicked();

    void on_back_clicked();

private:
    Ui::WithdrawalScreen *ui;
    std::function<void(WithdrawalRequest, WithdrawalDetails)> _toDetails;
    SignedConnection &_connection;
    std::function<void()> _back;
};

#endif // WITHDRAWALSCREEN_H
