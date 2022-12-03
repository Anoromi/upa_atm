#ifndef PUT_MONEY_H
#define PUT_MONEY_H

#include <QWidget>

namespace Ui {
class put_money;
}

class put_money : public QWidget
{
    Q_OBJECT

public:
    explicit put_money(QWidget *parent = nullptr);
    ~put_money();

private:
    Ui::put_money *ui;
};

#endif // PUT_MONEY_H
