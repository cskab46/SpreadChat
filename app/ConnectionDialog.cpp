#include "ConnectionDialog.h"
#include "ui_ConnectionDialog.h"

#include <QMessageBox>
#include "SpreadConnection.h"
#include "ChatWindow.h"

ConnectionDialog::ConnectionDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
#ifdef Q_OS_WIN
    QByteArray username = qgetenv("USERNAME");
#else
    QByteArray username = qgetenv("USER");
#endif
    ui->nameField->setText(username);
    ui->nameField->setFocus();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_buttonBox_accepted()
{
    const char* host = ui->hostField->text().toLatin1();
    const int port = ui->portField->text().toInt(nullptr);
    SpreadConnPtr conn = std::make_unique<SpreadConnection>(host, port);
    ChatWindow* window = new ChatWindow(std::move(conn));
    window->setAttribute(Qt::WA_DeleteOnClose, true);
    window->show();
    close();
}

void ConnectionDialog::on_buttonBox_rejected()
{
    close();
}
