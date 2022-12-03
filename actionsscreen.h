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

private slots:
    void on_transfer_clicked();

    void on_withdraw_clicked();

    void on_refil_clicked();

private:
    Ui::ActionsScreen *ui;
    Shared<SignedConnection> _connect;
    std::function<void(QWidget *destination)> _push;
    std::function<void()> _pop;



    void endSession();
};

#endif // ACTIONSSCREEN_H
