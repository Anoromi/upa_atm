#ifndef TRANSACTION_DETAILS_H
#define TRANSACTION_DETAILS_H

#include <QWidget>
#include "middleware/credentials.h"
#include "middleware/tariff.h"

namespace Ui {
    class transaction_details;
}

class transaction_details : public QWidget {
Q_OBJECT

public:
    explicit transaction_details(
            String message,
            std::optional<Card> receiver,
            Shared<Tariff> &tariff,
            uint money,
            std::function<void(bool)> move,
            QWidget *parent = nullptr
    );

    ~transaction_details();

private slots:

    void on_continue_2_clicked();

    void on_cancel_clicked();

private:
    Ui::transaction_details *ui;
    const String _message;
    const std::optional<Card> _receiver;
    const Shared<Tariff> _tariff;
    const uint _money;
    std::function<void(bool)> _navigateNext;


};

#endif // TRANSACTION_DETAILS_H
