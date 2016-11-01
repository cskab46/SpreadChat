#include "ConnectionDialog.h"
#include "ui_ConnectionDialog.h"

#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
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

void ConnectionDialog::setAllEnabled(bool enabled)
{
    ui->nameField->setEnabled(enabled);
    ui->hostField->setEnabled(enabled);
    ui->portField->setEnabled(enabled);
    ui->buttonBox->setEnabled(enabled);
}

static SpreadConnPtr globalStackConnection;

void ConnectionDialog::on_buttonBox_accepted()
{
    setAllEnabled(false);
    setCursor(Qt::WaitCursor);
    std::string user = ui->nameField->text().toStdString();
    std::string host = ui->hostField->text().toStdString();
    int port = ui->portField->text().toInt(nullptr);
    QtConcurrent::run([=]() {
        SpreadConnPtr connection = std::make_unique<SpreadConnection>(user, host, port);
        globalStackConnection = std::move(connection);
        QMetaObject::invokeMethod(this, "onConnect", Qt::QueuedConnection);
    });
}

void ConnectionDialog::on_buttonBox_rejected()
{
    close();
}

void ConnectionDialog::onConnect()
{
    SpreadConnPtr connection = std::move(globalStackConnection);
    if (connection->isConnected()) {
        ChatWindow* window = new ChatWindow(std::move(connection));
        window->setAttribute(Qt::WA_DeleteOnClose, true);
        window->showNormal();
        close();
    }
    else {
        int error = connection->getLastError();
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
        setAllEnabled(true);
        setCursor(Qt::ArrowCursor);
    }
}
