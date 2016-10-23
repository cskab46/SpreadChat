#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include <QMessageBox>
#include "spread/SpreadConnection.h"

ChatWindow::ChatWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
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
