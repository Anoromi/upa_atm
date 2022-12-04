#ifndef WITHDRAWALSCREEN_H
#define WITHDRAWALSCREEN_H

#include "middleware/signedConnection.h"
#include <QWidget>

namespace Ui {
class WithdrawalScreen;
}

class WithdrawalScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WithdrawalScreen(
            std::function<void(const WithdrawalRequest&, const WithdrawalDetails&)> proceed,
            std::function<void()> back,
            const SignedConnection& connetion,
            QWidget *parent = nullptr
            );
    ~WithdrawalScreen();

private slots:
    void on_confirm_clicked();

    void on_back_clicked();

private:
    std::function<void(const WithdrawalRequest&, const WithdrawalDetails&)> _proceed;
    std::function<void()> _back;
    SignedConnection _connection;
    Ui::WithdrawalScreen *ui;
};

#endif // WITHDRAWALSCREEN_H
