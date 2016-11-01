#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <map>
#include "SpreadConnection.h"

namespace Ui {
class ChatWindow;
}

class QComboBox;
class ChatTabWidget;

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(SpreadConnPtr connection, QWidget* parent = 0);
    ~ChatWindow();

    QTextCodec* getEncoding();

private slots:
    void on_actionAbout_triggered();
    void on_actionJoinGroup_triggered();
    void on_tabWidget_tabCloseRequested(int index);

    void receiveMessage(SpreadMessage message);

private:
    void createDefaultTab();
    void addGroupTab(SpreadGroup* group);

    Ui::ChatWindow* ui;
    QComboBox* codecBox;
    SpreadConnPtr connection;
    std::map<QByteArray, ChatTabWidget*> tabs;
    bool defaultTabVisible;
};

#endif // CHATWINDOW_H
