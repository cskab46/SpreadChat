#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include "SpreadConnection.h"

namespace Ui {
class ChatWindow;
}

class QComboBox;

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(SpreadConnPtr connection, QWidget* parent = 0);
    ~ChatWindow();

    QTextCodec* getInputEncoding();
    QTextCodec* getOutputEncoding();

private slots:
    void on_actionAbout_triggered();
    void on_actionJoinGroup_triggered();
    void on_tabWidget_tabCloseRequested(int index);

private:
    void createDefaultTab();
    void addGroupTab(SpreadGroup* group);

    Ui::ChatWindow* ui;
    QComboBox* inCodec;
    QComboBox* outCodec;
    SpreadConnPtr connection;
    bool defaultTabVisible;
};

#endif // CHATWINDOW_H
