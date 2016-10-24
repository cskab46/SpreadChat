#include "JoinDialog.h"
#include "ui_JoinDialog.h"

JoinDialog::JoinDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::JoinDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

JoinDialog::~JoinDialog()
{
    delete ui;
}

QString JoinDialog::groupName() const
{
    return ui->lineEdit->text();
}

void JoinDialog::on_buttonBox_accepted()
{
    accept();
}

void JoinDialog::on_buttonBox_rejected()
{
    reject();
}
