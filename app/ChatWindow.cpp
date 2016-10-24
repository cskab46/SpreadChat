#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "JoinDialog.h"
#include "SpreadConnection.h"

ChatWindow::ChatWindow(SpreadConnPtr conn, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
    , connection(std::move(conn))
{
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString(connection->getHostname()));
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_actionAbout_triggered()
{
    struct StaticConstructor {
        QString str;
        StaticConstructor(QString start, std::string version)
            : str(start + QString::fromStdString(version))
        {}
    };

    const static StaticConstructor staticText {
        "Este produto utiliza software desenvolvido pela Spread "
        "Concepts LLC para uso junto ao Spread toolkit. Para mais informações sobre o "
        "Spread, visite <a href='http://www.spread.org/'>http://www.spread.org/</a><br>"
        "<hr><br>"
        "Versão do Spread: ", SpreadConnection::getVersion()
    };

    QMessageBox about(this);
    about.setWindowTitle("Sobre");
    about.setTextFormat(Qt::RichText);
    about.setIcon(QMessageBox::Information);
    about.setText(staticText.str);
    about.exec();
}

void ChatWindow::on_actionJoinGroup_triggered()
{
    JoinDialog dialog;
    while (dialog.exec()) {
        QByteArray groupName = dialog.groupName().toLatin1();
        int status = connection->joinGroup(groupName.data());
        if (status != 0) {
            QMessageBox::critical(this, "Erro", "Nome de grupo inválido", QMessageBox::Ok);
        }
        else {
            break;
        }
    }
}
