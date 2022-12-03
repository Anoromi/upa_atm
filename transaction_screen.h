#ifndef TRANSACTION_SCREEN_H
#define TRANSACTION_SCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"
#include "actionsscreen.h"
#include <functional>

namespace Ui {
class TransactionScreen;
}

class TransactionScreen : public QWidget {
    Q_OBJECT

public:

    TransactionScreen(QWidget *parent, const SignedConnection& connection,
                       const std::function<void(QWidget*)>& toDetails);

//    explicit transaction_screen(QWidget *parent = nullptr, Shared<std::function>);
    ~TransactionScreen();

private slots:

    void on_submitButton_clicked();

private:

    void toDetails(const TransferRequest&, const TransferDetails&);

    Ui::TransactionScreen *ui;
    SignedConnection _connection;
    std::function<void(QWidget* push)> _mainMenuPush;
    
};

#endif // TRANSACTION_SCREEN_H
