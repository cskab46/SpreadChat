#ifndef CHATTABWIDGET_H
#define CHATTABWIDGET_H

#include <QWidget>

class SpreadGroup;

namespace Ui {
class ChatTabWidget;
}

class ChatTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatTabWidget(const SpreadGroup* group, QWidget* parent = 0);
    ~ChatTabWidget();

    const SpreadGroup* getGroup() const;

private:
    Ui::ChatTabWidget* ui;
    const SpreadGroup* group;
};

#endif // CHATTABWIDGET_H
