#ifndef TRANSACTIONITEM_H
#define TRANSACTIONITEM_H

#include <QWidget>
#include "middleware/transaction.h"

namespace Ui {
    class TransactionItem;
}

class TransactionItem : public QWidget {
    Q_OBJECT

public:
    explicit TransactionItem(const Transaction& t, QWidget *parent = nullptr);

    ~TransactionItem();

private:
    Ui::TransactionItem *ui;
    Transaction _transaction;
};

#endif // TRANSACTIONITEM_H
