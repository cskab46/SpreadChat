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
    void addNotification(QByteArray user, QByteArray text);
    void refreshMessages();
    void setFocus();

private slots:
    void on_sendButton_clicked();

private:
    void addMessageImpl(SpreadMessage message);
    void addNotificationImpl(QByteArray user, QByteArray text);

    Ui::ChatTabWidget* ui;
    SpreadConnPtr& connection;
    QByteArray groupName;
    ChatWindow* window;
    std::vector<SpreadMessage> messageLog;
};

#endif // CHATTABWIDGET_H
