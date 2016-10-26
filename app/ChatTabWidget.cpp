#include "ChatTabWidget.h"
#include "ui_ChatTabWidget.h"

#include <QEvent>
#include <QKeyEvent>
#include <QAbstractButton>
#include <QTextCodec>
#include "ChatWindow.h"
#include "SpreadGroup.h"

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
            bool enterPressed = keyEvent->key() == Qt::Key_Return;
            bool shiftPressed = keyEvent->modifiers() == Qt::ShiftModifier;
            if (enterPressed && !shiftPressed) {
                button->click();
                return true;
            }
        }
        return QObject::eventFilter(object, event);
    }
};

ChatTabWidget::ChatTabWidget(SpreadGroup* group, ChatWindow* parent)
    : QWidget(parent)
    , ui(new Ui::ChatTabWidget)
    , group(group)
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

const SpreadGroup* ChatTabWidget::getGroup() const
{
    return group;
}

void ChatTabWidget::setFocus()
{
    ui->inputField->setFocus();
}

void ChatTabWidget::on_sendButton_clicked()
{
    const QString& text = ui->inputField->toPlainText();
    if (!text.isEmpty()) {
        QTextCodec* codec = window->getOutputEncoding();
        QByteArray message = codec->fromUnicode(text);
        group->sendMessage(message);
        ui->inputField->clear();
    }
}
