#ifndef NAVIGATIONWINDOW_H
#define NAVIGATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class NavigationWindow;
}

class NavigationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NavigationWindow(QWidget *parent = nullptr);
    ~NavigationWindow();

private:
    Ui::NavigationWindow *ui;
};

#endif // NAVIGATIONWINDOW_H
