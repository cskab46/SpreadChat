#ifndef SPREADWORKER_H
#define SPREADWORKER_H

#include <QThread>
#include <QByteArray>
#include "SpreadMessage.h"

class SpreadWorker : public QThread
{
    Q_OBJECT

    volatile bool running;

public:
    explicit SpreadWorker(QObject* parent = 0);
    ~SpreadWorker();

    virtual void run() override;
    Q_INVOKABLE void finish();

    int mailbox;

signals:
    void userJoined(QByteArray group, QByteArray name);
    void userLeft(QByteArray group, QByteArray name);
    void messageReceived(SpreadMessage message);
};

#endif // SPREADWORKER_H
