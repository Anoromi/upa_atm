#ifndef PHONEWINDOW_H
#define PHONEWINDOW_H

#include <QWidget>
#include "middleware/signedConnection.h"

namespace Ui {
class PhoneWindow;
}

class PhoneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneWindow(
            std::function<void(const WithdrawalRequest&, const WithdrawalDetails&)> proceed,
            std::function<void()> back,
            const SignedConnection& connetion,
            QWidget *parent = nullptr
            );
    ~PhoneWindow();

private slots:
    void on_confirm_clicked();

    void on_cancel_clicked();

private:
    std::function<void(const WithdrawalRequest&, const WithdrawalDetails&)> _proceed;
    std::function<void()> _back;
    SignedConnection _connection;
    Ui::PhoneWindow *ui;
};

#endif // PHONEWINDOW_H
