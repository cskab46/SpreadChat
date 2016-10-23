#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include <QMessageBox>

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
    const static QString aboutText =
        "Este produto utiliza software desenvolvido pela Spread "
        "Concepts LLC para uso junto ao Spread toolkit. Para mais informações sobre o "
        "Spread, visite <a href='http://www.spread.org/'>http://www.spread.org/</a><br>";
    QMessageBox about(this);
    about.setWindowTitle("Sobre");
    about.setTextFormat(Qt::RichText);
    about.setIcon(QMessageBox::Information);
    about.setText(aboutText);
    about.exec();
}
