#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include <QComboBox>
#include <QTextCodec>
#include <QCompleter>
#include <QDebug>
#include "JoinDialog.h"
#include "ChatTabWidget.h"
#include "SpreadConnection.h"

Q_DECLARE_METATYPE(QTextCodec*);
Q_DECLARE_METATYPE(SpreadMessage);

static QList<QTextCodec*> getSystemCodecs()
{
    QList<int> codecMibs = QTextCodec::availableMibs();
    QList<QTextCodec*> codecs;
    codecs.reserve(codecMibs.size());
    for(int mib : codecMibs) {
        codecs.push_back(QTextCodec::codecForMib(mib));
    }
    qSort(codecs.begin(), codecs.end(), [](const QTextCodec* a, const QTextCodec* b) {
        return a->name().toLower() < b->name().toLower();
    });
    return codecs;
}

ChatWindow::ChatWindow(SpreadConnPtr conn, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatWindow)
    , connection(std::move(conn))
    , defaultTabVisible(false)
{
    ui->setupUi(this);

    QToolBar* toolbar = ui->toolBar;
    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    codecBox = new QComboBox(this);
    codecBox->setMaxVisibleItems(100);
    codecBox->setMaximumWidth(120);

    QStringList codecNames;
    QList<QTextCodec*> codecs = getSystemCodecs();

    for(QTextCodec* codec : codecs) {
        QByteArray codecName = codec->name();
        if (!codecNames.contains(codecName)) {
            QVariant codecProxy = QVariant::fromValue<QTextCodec*>(codec);
            codecBox->addItem(codecName, codecProxy);
            codecNames.append(codecName);
        }
    }
    codecBox->setCurrentText("UTF-16");

    connect(codecBox, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshAllTabs()));

    toolbar->addWidget(spacer);
    toolbar->addWidget(codecBox);

    setWindowTitle(QString("Chat em %1").arg(connection->getHostname()));
    createDefaultTab();

    connect(connection->getWorker(), &SpreadWorker::messageReceived,
            this, &ChatWindow::receiveMessage, Qt::QueuedConnection);

    connect(connection->getWorker(), &SpreadWorker::userJoined,
            this, &ChatWindow::notifyJoin, Qt::QueuedConnection);

    connect(connection->getWorker(), &SpreadWorker::userLeft,
            this, &ChatWindow::notifyLeave, Qt::QueuedConnection);

    connect(connection->getWorker(), &SpreadWorker::fatalError,
            this, &ChatWindow::quitWithError, Qt::QueuedConnection);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

QTextCodec* ChatWindow::getEncoding()
{
    return codecBox->currentData().value<QTextCodec*>();
}

void ChatWindow::on_actionAbout_triggered()
{
    const static QString versionString = QString("%1 %2").arg(
        "Este produto utiliza software desenvolvido pela Spread "
        "Concepts LLC para uso junto ao Spread toolkit. Para mais informações sobre o "
        "Spread, visite <a href='http://www.spread.org/'>http://www.spread.org/</a><br>"
        "<hr><br>"
        "Versão do Spread: "
    )
    .arg(SpreadConnection::getVersion());

    QMessageBox about(this);
    about.setWindowTitle("Sobre");
    about.setTextFormat(Qt::RichText);
    about.setIcon(QMessageBox::Information);
    about.setText(versionString);
    about.exec();
}

void ChatWindow::on_actionJoinGroup_triggered()
{
    JoinDialog dialog;
    while (dialog.exec()) {
        QByteArray groupName = dialog.groupName().toLatin1();
        if (!connection->inGroup(groupName.data())) {
            // Entra no grupo novo
            bool joined = connection->joinGroup(groupName.data());
            if (joined) {
                addGroupTab(groupName);
                break;
            }
            else {
                QMessageBox::critical(this, "Erro", "Nome de grupo inválido", QMessageBox::Ok);
            }
        }
        else {
            // Seleciona o grupo existente
            int index = 0;
            while (index < ui->tabWidget->count()) {
                ChatTabWidget* widget = dynamic_cast<ChatTabWidget*>(ui->tabWidget->widget(index));
                QByteArray name = widget->getGroupName();
                if (name == groupName.data()) {
                    break;
                }
                index++;
            }
            ui->tabWidget->setCurrentIndex(index);
            break;
        }
    }
}

void ChatWindow::on_tabWidget_tabCloseRequested(int index)
{
    ChatTabWidget* widget = dynamic_cast<ChatTabWidget*>(ui->tabWidget->widget(index));
    QByteArray group = widget->getGroupName();
    connection->leaveGroup(group);
    auto tab = tabs.find(group);
    tabs.erase(tab);
    ui->tabWidget->removeTab(index);
    if (ui->tabWidget->count() == 0) {
        createDefaultTab();
    }
}

void ChatWindow::receiveMessage(SpreadMessage message)
{
    ChatTabWidget* tab = getTab(message.group);
    if (tab) {
        tab->addMessage(message);
    }
    else {
        QMessageBox::warning(this, "Aviso", tr("Mensagem encaminhada para grupo inválido: %1").arg(QString(message.group)), QMessageBox::Ok);
    }
}

void ChatWindow::notifyJoin(QByteArray group, QByteArray user)
{
    ChatTabWidget* tab = getTab(group);
    if (tab) {
        tab->addNotification(user, "entrou no grupo", Qt::darkGreen);
    }
}

void ChatWindow::notifyLeave(QByteArray group, QByteArray user)
{
    ChatTabWidget* tab = getTab(group);
    if (tab) {
        tab->addNotification(user, "saiu do grupo", Qt::darkRed);
    }
}

void ChatWindow::quitWithError(QString err)
{
    hide();
    QMessageBox::critical(this, "Erro no worker", err, QMessageBox::Abort);
    qApp->exit(1);
}

void ChatWindow::addGroupTab(QByteArray groupName)
{
    if (defaultTabVisible) {
        ui->tabWidget->removeTab(0);
        ui->tabWidget->setEnabled(true);
        defaultTabVisible = false;
    }
    auto widget = new ChatTabWidget(connection, groupName, this);
    ui->tabWidget->addTab(widget, groupName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    tabs.insert({groupName, widget});
    widget->setFocus();
}

ChatTabWidget* ChatWindow::getTab(QByteArray group) const
{
    auto iterPair = tabs.find(group);
    if (iterPair != tabs.end()) {
        return iterPair->second;
    }
    else {
        return nullptr;
    }
}

void ChatWindow::refreshAllTabs()
{
    for (auto pair : tabs) {
        pair.second->refreshMessages();
    }
}

void ChatWindow::createDefaultTab()
{
    ui->tabWidget->addTab(new QWidget(this), "Nenhum grupo");
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setEnabled(false);
    defaultTabVisible = true;
}
