#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget* parent = 0);
    ~ChatWindow();

private slots:
    void on_actionAbout_triggered();

private:
    Ui::ChatWindow* ui;
};

#endif // CHATWINDOW_H
