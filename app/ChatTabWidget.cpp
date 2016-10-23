#include "ChatTabWidget.h"
#include "ui_ChatTabWidget.h"

ChatTabWidget::ChatTabWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatTabWidget)
{
    ui->setupUi(this);
}

ChatTabWidget::~ChatTabWidget()
{
    delete ui;
}
