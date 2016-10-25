#include "ChatTabWidget.h"
#include "ui_ChatTabWidget.h"

#include "SpreadGroup.h"

ChatTabWidget::ChatTabWidget(const SpreadGroup* group, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatTabWidget)
    , group(group)
{
    ui->setupUi(this);
}

ChatTabWidget::~ChatTabWidget()
{
    delete ui;
}

const SpreadGroup* ChatTabWidget::getGroup() const
{
    return group;
}
