#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "middleware/signedConnection.h"
#include "middleware/unsignedConnection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow {
Q_OBJECT

public:
    Login(UnsignedConnection uConnection,
          std::function<void(Shared<SignedConnection>)> enter,
          QWidget *parent = nullptr);

    ~Login();

private slots:

    void on_confirmLogin_clicked();

private:
    Ui::Login *ui;
    std::function<void(Shared<SignedConnection>)> _enter;
    UnsignedConnection _uConnection;

};

#endif // LOGIN_H
