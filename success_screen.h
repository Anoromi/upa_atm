#ifndef SUCCESS_SCREEN_H
#define SUCCESS_SCREEN_H

#include <QWidget>

namespace Ui {
    class success_screen;
}

class success_screen : public QWidget {
Q_OBJECT

public:
    explicit success_screen(std::function<void()> end, QWidget *parent = nullptr);

    ~success_screen();

private slots:
    void on_end_button_clicked();

private:
    Ui::success_screen *ui;
    std::function<void()> _end;
};

#endif // SUCCESS_SCREEN_H
