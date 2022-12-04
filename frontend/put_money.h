#ifndef PUT_MONEY_H
#define PUT_MONEY_H

#include <QWidget>
#include <middleware/signedConnection.h>

namespace Ui {
class put_money;
}

class put_money : public QWidget
{
    Q_OBJECT

public:
    explicit put_money(
            QWidget *parent = nullptr
            );
    ~put_money();

private slots:
    void on_put_clicked();

    void on_back_clicked();

private:
    Ui::put_money *ui;
};

#endif // PUT_MONEY_H
