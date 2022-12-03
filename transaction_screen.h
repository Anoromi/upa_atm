#ifndef TRANSACTION_SCREEN_H
#define TRANSACTION_SCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"
#include "actionsscreen.h"
#include <functional>

namespace Ui {
    class TransactionScreen;
}

class TransactionScreen : public QWidget {
Q_OBJECT


public:

    TransactionScreen(SignedConnection &connection, std::function<void()> back,
                      std::function<void(TransferRequest, TransferDetails)> toDetails,
                      QWidget *parent);

    ~TransactionScreen();

private slots:

    void on_submitButton_clicked();

private:

    void toDetails(const TransferRequest &, const TransferDetails &);

    Ui::TransactionScreen *ui;
    SignedConnection _connection;
    std::function<void(TransferRequest, TransferDetails)> _toDetails;
    std::function<void()> _back;
};

#endif // TRANSACTION_SCREEN_H
