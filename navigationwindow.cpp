#include "navigationwindow.h"
#include "ui_navigationwindow.h"

NavigationWindow::NavigationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NavigationWindow)
{
    ui->setupUi(this);
}

NavigationWindow::~NavigationWindow()
{
    delete ui;
}
