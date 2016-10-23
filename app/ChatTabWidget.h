#ifndef CHATTABWIDGET_H
#define CHATTABWIDGET_H

#include <QWidget>

namespace Ui {
class ChatTabWidget;
}

class ChatTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatTabWidget(QWidget* parent = 0);
    ~ChatTabWidget();

private:
    Ui::ChatTabWidget* ui;
};

#endif // CHATTABWIDGET_H
