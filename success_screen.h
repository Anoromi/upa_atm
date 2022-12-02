#ifndef SUCCESS_SCREEN_H
#define SUCCESS_SCREEN_H

#include <QWidget>

namespace Ui {
class success_screen;
}

class success_screen : public QWidget
{
    Q_OBJECT

public:
    explicit success_screen(QWidget *parent = nullptr);
    ~success_screen();

private:
    Ui::success_screen *ui;
};

#endif // SUCCESS_SCREEN_H
