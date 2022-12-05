#ifndef PUT_MONEY_H
#define PUT_MONEY_H

#include <QWidget>
#include "middleware/signedConnection.h"

namespace Ui {
    class PutMoney;
}

class PutMoney : public QWidget
{
    Q_OBJECT

public:
    explicit PutMoney(
            std::function<void(const DepositRequest&, const DepositDetails&)> proceed,
            std::function<void()> back,
            const SignedConnection& connection,
            QWidget *parent = nullptr
            );
    ~PutMoney();

private slots:
    void on_put_clicked();

    void on_back_clicked();

private:
    std::function<void(const DepositRequest&, const DepositDetails&)> _proceed;
    std::function<void()> _back;
    SignedConnection _connection;
    Ui::PutMoney *ui;
};

#endif // PUT_MONEY_H
