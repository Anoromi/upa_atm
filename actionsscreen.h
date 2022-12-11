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

    void on_refilMobile_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ActionsScreen *ui;
    Shared<SignedConnection> _connect;
    std::function<void(QWidget *destination)> _push;
    std::function<void()> _pop;

    void updateCardInfo();


    void endSession();


    void toDetails(
            String message,
            std::optional<Card> receiver,
            const Shared<Tariff> &tariff,
            uint money,
            std::function<void()> performAction
    );

};

#endif // ACTIONSSCREEN_H
