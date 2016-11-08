#ifndef CHATTABWIDGET_H
#define CHATTABWIDGET_H

#include <QWidget>
#include "SpreadConnection.h"
#include "SpreadMessage.h"

namespace Ui {
class ChatTabWidget;
}

class ChatWindow;

class ChatTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatTabWidget(SpreadConnPtr& connection, QByteArray groupName, ChatWindow* parent);
    ~ChatTabWidget();

    QByteArray getGroupName() const;
    void addMessage(SpreadMessage message);
    void addNotification(QString text);
    void setFocus();

private slots:
    void on_sendButton_clicked();

private:
    Ui::ChatTabWidget* ui;
    SpreadConnPtr& connection;
    QByteArray groupName;
    ChatWindow* window;
};

#endif // CHATTABWIDGET_H
