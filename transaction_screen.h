#ifndef TRANSACTION_SCREEN_H
#define TRANSACTION_SCREEN_H

#include <QWidget>
#include "middleware/signedConnection.h"
#include <functional>

namespace Ui {
class transaction_screen;
}

class transaction_screen : public QWidget
{
    Q_OBJECT


public:
    transaction_screen(QWidget *parent, const SignedConnection &connection,
                       std::function<void(TransferRequest, TransferDetails)> toDetails);

//    explicit transaction_screen(QWidget *parent = nullptr, Shared<std::function>);
    ~transaction_screen();

private slots:
    void on_pushButton_clicked();
private:
    Ui::transaction_screen *ui;
    SignedConnection _connection;
    std::function<void(TransferRequest, TransferDetails)> _moveToDetails;
};

#endif // TRANSACTION_SCREEN_H
