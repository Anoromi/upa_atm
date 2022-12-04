#ifndef TRANSACTION_SCREEN_H
#define TRANSACTION_SCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"
#include <functional>

namespace Ui {
    class TransactionScreen;
}

class TransactionScreen : public QWidget {
Q_OBJECT

public:

    TransactionScreen(QWidget *parent, const SignedConnection &connection,
                      std::function<void(TransferRequest, TransferDetails)> toDetails,
                      std::function<void()> back
    );

    ~TransactionScreen();

private slots:

    void on_submitButton_clicked();

    void on_back_clicked();

private:


    Ui::TransactionScreen *ui;
    SignedConnection _connection;
    std::function<void(TransferRequest, TransferDetails)> _toDetails;
    std::function<void()> _back;
};

#endif // TRANSACTION_SCREEN_H
