#include "ChatTabWidget.h"
#include "ui_ChatTabWidget.h"

#include "SpreadGroup.h"

ChatTabWidget::ChatTabWidget(const SpreadGroup* group, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatTabWidget)
    , group(group)
{
    ui->setupUi(this);
    ui->splitter->setCollapsible(1, false);
    ui->splitter->setStretchFactor(0, 100);
    ui->splitter->setStretchFactor(1,   1);
}

ChatTabWidget::~ChatTabWidget()
{
    delete ui;
}

const SpreadGroup* ChatTabWidget::getGroup() const
{
    return group;
}

void ChatTabWidget::setFocus()
{
    ui->messagePanel->setFocus();
}
