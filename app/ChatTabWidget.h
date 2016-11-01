#ifndef CHATTABWIDGET_H
#define CHATTABWIDGET_H

#include <QWidget>
#include "SpreadMessage.h"

class SpreadGroup;

namespace Ui {
class ChatTabWidget;
}

class ChatWindow;

class ChatTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatTabWidget(SpreadGroup* group, ChatWindow* parent);
    ~ChatTabWidget();

    const SpreadGroup* getGroup() const;
    void addMessage(SpreadMessage message);
    void setFocus();

private slots:
    void on_sendButton_clicked();

private:
    Ui::ChatTabWidget* ui;
    SpreadGroup* group;
    ChatWindow* window;
};

#endif // CHATTABWIDGET_H
