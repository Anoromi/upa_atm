#ifndef PHONEWINDOW_H
#define PHONEWINDOW_H

#include <QWidget>

namespace Ui {
class PhoneWindow;
}

class PhoneWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneWindow(QWidget *parent = nullptr);
    ~PhoneWindow();

private:
    Ui::PhoneWindow *ui;
};

#endif // PHONEWINDOW_H
