#ifndef ACTIONSSCREEN_H
#define ACTIONSSCREEN_H

#include <QWidget>

namespace Ui {
class ActionsScreen;
}

class ActionsScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ActionsScreen(QWidget *parent = nullptr);
    ~ActionsScreen();

private:
    Ui::ActionsScreen *ui;
};

#endif // ACTIONSSCREEN_H
