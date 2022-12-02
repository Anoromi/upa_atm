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
    explicit ActionsScreen(Shared<SignedConnection>& s, QWidget *parent = nullptr);

    ~ActionsScreen();

private:
    Ui::ActionsScreen *ui;
    Shared<SignedConnection> s;
};

#endif // ACTIONSSCREEN_H
