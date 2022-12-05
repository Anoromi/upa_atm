#ifndef TRANSACTIONHISTORYSCREEN_H
#define TRANSACTIONHISTORYSCREEN_H

#include <QWidget>

namespace Ui {
class TransactionHistoryScreen;
}

class TransactionHistoryScreen : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionHistoryScreen(QWidget *parent = nullptr);
    ~TransactionHistoryScreen();

private:
    Ui::TransactionHistoryScreen *ui;
};

#endif // TRANSACTIONHISTORYSCREEN_H
