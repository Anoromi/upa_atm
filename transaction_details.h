#ifndef TRANSACTION_DETAILS_H
#define TRANSACTION_DETAILS_H

#include <QWidget>
#include "middleware/credentials.h"
#include "middleware/tariff.h"

namespace Ui {
    class TransactionDetails;
}

class TransactionDetails : public QWidget {
Q_OBJECT

public:
    explicit TransactionDetails(
            String message,
            std::optional<Card> receiver,
            const Shared<Tariff> tariff,
            uint money,
            std::function<void(bool)> move,
            QWidget *parent = nullptr
    );

    ~TransactionDetails();

private slots:

    void on_continue_2_clicked();

    void on_cancel_clicked();

private:
    Ui::TransactionDetails *ui;
    const String _message;
    const std::optional<Card> _receiver;
    const Shared<Tariff> _tariff;
    const uint _money;
    std::function<void(bool)> _navigateNext;


};

#endif // TRANSACTION_DETAILS_H
