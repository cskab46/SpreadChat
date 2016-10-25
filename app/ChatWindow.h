#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include "SpreadConnection.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(SpreadConnPtr connection, QWidget* parent = 0);
    ~ChatWindow();

private slots:
    void on_actionAbout_triggered();
    void on_actionJoinGroup_triggered();

private:
    Ui::ChatWindow* ui;
    SpreadConnPtr connection;
};

#endif // CHATWINDOW_H
