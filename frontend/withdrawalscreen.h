#ifndef WITHDRAWALSCREEN_H
#define WITHDRAWALSCREEN_H

#include <QWidget>

namespace Ui {
class WithdrawalScreen;
}

class WithdrawalScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WithdrawalScreen(QWidget *parent = nullptr);
    ~WithdrawalScreen();

private:
    Ui::WithdrawalScreen *ui;
};

#endif // WITHDRAWALSCREEN_H