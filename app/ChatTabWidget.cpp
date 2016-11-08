#include "ChatTabWidget.h"
#include "ui_ChatTabWidget.h"

#include <QEvent>
#include <QKeyEvent>
#include <QAbstractButton>
#include <QShortcut>
#include <QTextCodec>
#include <QTextEdit>

#include "ChatWindow.h"

class TextEditSubmitFilter : public QObject
{
public:
    explicit TextEditSubmitFilter(QAbstractButton* button, QObject* parent = 0)
        : QObject(parent)
        , button(button)
    {}

    virtual ~TextEditSubmitFilter()
    {}
protected:
    QAbstractButton* button;

    bool eventFilter(QObject* object, QEvent* event)
    {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            bool enterPressed = keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter;
            bool shiftPressed = keyEvent->modifiers() == Qt::ShiftModifier;
            if (enterPressed && !shiftPressed) {
                button->click();
                return true;
            }
        }
        return QObject::eventFilter(object, event);
    }
};

ChatTabWidget::ChatTabWidget(SpreadConnPtr& connection, QByteArray groupName, ChatWindow* parent)
    : QWidget(parent)
    , ui(new Ui::ChatTabWidget)
    , connection(connection)
    , groupName(groupName)
    , window(parent)
{
    ui->setupUi(this);
    ui->splitter->setCollapsible(1, false);
    ui->splitter->setStretchFactor(0, 100);
    ui->splitter->setStretchFactor(1,   1);
    ui->inputField->installEventFilter(new TextEditSubmitFilter(ui->sendButton, this));
}

ChatTabWidget::~ChatTabWidget()
{
    delete ui;
}

QByteArray ChatTabWidget::getGroupName() const
{
    return groupName;
}

void ChatTabWidget::addMessage(SpreadMessage message)
{
    addMessageImpl(message);
    messageLog.push_back({message, Qt::black});
}

void ChatTabWidget::addNotification(QByteArray user, QByteArray text, QColor color)
{
    addNotificationImpl(user, text, color);
    SpreadMessage message = {"", user, text};
    messageLog.push_back({message, color});
}

void ChatTabWidget::refreshMessages()
{
    ui->outputLog->clear();
    for (auto item : messageLog) {
        auto msg = std::get<0>(item);
        auto color = std::get<1>(item);
        // Mensagem de usuário
        if (!msg.group.isEmpty()) {
            addMessageImpl(msg);
        }
        // Notificação do grupo
        else {
            addNotificationImpl(msg.user, msg.text, color);
        }
    }
}

void ChatTabWidget::setFocus()
{
    ui->inputField->setFocus();
}

void ChatTabWidget::on_sendButton_clicked()
{
    const QString& text = ui->inputField->toPlainText();
    if (!text.isEmpty()) {
        QTextCodec* codec = window->getEncoding();
        QByteArray message = codec->fromUnicode(text);
        connection->sendMessage(groupName, message);
        ui->inputField->clear();
    }
}

void ChatTabWidget::addMessageImpl(SpreadMessage message)
{
    QTextCodec* codec = window->getEncoding();
    QString text = codec->toUnicode(message.text);
    QString user = QString(message.user).section("#", 1, 1);
    QTextEdit* log = ui->outputLog;
    log->setFontWeight(QFont::Bold);
    log->setTextColor(Qt::darkCyan);
    log->append(user + ":");
    log->setFontWeight(QFont::Normal);
    log->setTextColor(Qt::black);
    log->append(text);
    log->append("");
}

void ChatTabWidget::addNotificationImpl(QByteArray user, QByteArray text, QColor color)
{
    QString notification = QString("%1 %2").arg(QString(user)).arg(QString(text));
    QTextEdit* log = ui->outputLog;
    log->setTextColor(color);
    log->append(notification);
    log->append("");
}
