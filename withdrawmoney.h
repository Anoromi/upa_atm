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
    explicit WithdrawMoney(QWidget *parent = nullptr);
    ~WithdrawMoney();

private:
    Ui::WithdrawMoney *ui;
};

#endif // WITHDRAWMONEY_H
