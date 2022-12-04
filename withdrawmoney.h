#ifndef WITHDRAWMONEY_H
#define WITHDRAWMONEY_H

#include <QWidget>

namespace Ui {
class WithdrawMoney;
}

class WithdrawMoney : public QWidget
{
    Q_OBJECT

public:
    explicit WithdrawMoney(std::function<void()> end, QWidget *parent = nullptr);
    ~WithdrawMoney();

private:
    Ui::WithdrawMoney *ui;
    std::function<void()> _end;
};

#endif // WITHDRAWMONEY_H
