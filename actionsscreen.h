#ifndef ACTIONSSCREEN_H
#define ACTIONSSCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"

namespace Ui {
    class ActionsScreen;
}

class ActionsScreen : public QWidget {
Q_OBJECT

public:
    explicit ActionsScreen(Shared<SignedConnection> &s,
                           std::function<void(QWidget *destination)> push,
                           std::function<void()> pop, QWidget *parent = nullptr);

    ~ActionsScreen();

private:
    Ui::ActionsScreen *ui;
    Shared<SignedConnection> s;
    std::function<void(QWidget *destination)> _push;
    std::function<void()> _pop;

    void endSession();
};

#endif // ACTIONSSCREEN_H
