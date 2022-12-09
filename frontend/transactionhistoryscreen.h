#ifndef TRANSACTIONHISTORYSCREEN_H
#define TRANSACTIONHISTORYSCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"

namespace Ui {
    class TransactionHistoryScreen;
}

class TransactionHistoryScreen : public QWidget {

Q_OBJECT

public:
    TransactionHistoryScreen(SignedConnection &connection, std::function<void()> back, QWidget *parent = nullptr);

    ~TransactionHistoryScreen();

private slots:

    void on_pushButton_clicked();

private:
    Ui::TransactionHistoryScreen *ui;
    SignedConnection &_connection;
    std::function<void()> _back;
};

#endif // TRANSACTIONHISTORYSCREEN_H
