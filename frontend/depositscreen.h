#ifndef DEPOSITSCREEN_H
#define DEPOSITSCREEN_H

#include <QWidget>

namespace Ui {
class DepositScreen;
}

class DepositScreen : public QWidget
{
    Q_OBJECT

public:
    explicit DepositScreen(QWidget *parent = nullptr);
    ~DepositScreen();

private:
    Ui::DepositScreen *ui;
};

#endif // DEPOSITSCREEN_H
