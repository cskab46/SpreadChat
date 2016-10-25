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
    setWindowFlags(Qt::Window);
#ifdef Q_OS_WIN
    QByteArray username = qgetenv("USERNAME");
#else
    QByteArray username = qgetenv("USER");
#endif
    username.replace(" ", "");
    ui->nameField->setText(username);
    ui->nameField->setFocus();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_buttonBox_accepted()
{
    QByteArray user = ui->nameField->text().toLatin1();
    QByteArray host = ui->hostField->text().toLatin1();
    const int port = ui->portField->text().toInt(nullptr);
    SpreadConnPtr conn = std::make_unique<SpreadConnection>();
    conn->connect(user.data(), host.data(), port);
    if (conn->isConnected()) {
        ChatWindow* window = new ChatWindow(std::move(conn));
        window->setAttribute(Qt::WA_DeleteOnClose, true);
        window->showNormal();
        close();
    }
    else {
        int error = conn->getLastError();
        QString msg;
        switch (error) {
            case SpreadConnection::ILLEGAL_SPREAD:
            case SpreadConnection::COULD_NOT_CONNECT:
            case SpreadConnection::CONNECTION_CLOSED:
                msg = "Não foi possível conectar ao servidor, verifique se o endereço está correto";
                break;
            case SpreadConnection::REJECT_VERSION:
                msg = "A versão do servidor é incompatível com o cliente";
                break;
            case SpreadConnection::REJECT_NO_NAME:
            case SpreadConnection::REJECT_ILLEGAL_NAME:
                msg = "O nome de usuário fornecido é inválido";
                break;
            case SpreadConnection::REJECT_NOT_UNIQUE:
                msg = "O nome de usuário já existe no servidor e não é único";
                break;
            default:
                msg = "Erro não específico";
                break;
        }
        QMessageBox::critical(this, "Erro", msg, QMessageBox::Ok);
    }
}

void ConnectionDialog::on_buttonBox_rejected()
{
    close();
}
