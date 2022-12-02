#ifndef WITHDRAWSCREEN_H
#define WITHDRAWSCREEN_H

#include <QWidget>

namespace Ui {
class WithdrawScreen;
}

class WithdrawScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WithdrawScreen(QWidget *parent = nullptr);
    ~WithdrawScreen();

private:
    Ui::WithdrawScreen *ui;
};

#endif // WITHDRAWSCREEN_H
