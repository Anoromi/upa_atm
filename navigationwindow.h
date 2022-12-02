#ifndef NAVIGATIONWINDOW_H
#define NAVIGATIONWINDOW_H

#include <QMainWindow>
#include "middleware/signedConnection.h"

namespace Ui {
    class NavigationWindow;
}

class NavigationWindow : public QMainWindow {
Q_OBJECT

public:
    explicit NavigationWindow(QWidget *parent = nullptr);

    ~NavigationWindow();

private:
    Ui::NavigationWindow *ui;

    void pushNav(QWidget *);

    void popNav();

    void pushLoginScreen();

    void pushActionsScreen(Shared<SignedConnection> s);

    void pushSuccessScreen();

    void pushDetailsScreen();


};

#endif // NAVIGATIONWINDOW_H
