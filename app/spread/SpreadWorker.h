#ifndef SPREADWORKER_H
#define SPREADWORKER_H

#include <QThread>
#include <QByteArray>
#include <QMutex>
#include <functional>
#include "SpreadMessage.h"

class SpreadConnection;

class SpreadWorker : public QThread
{
    Q_OBJECT

    friend class SpreadConnection;

public:
    explicit SpreadWorker(QObject* parent = 0);
    ~SpreadWorker();

    virtual void run() override;
    Q_INVOKABLE void finish();

signals:
    void userJoined(QByteArray group, QByteArray name);
    void userLeft(QByteArray group, QByteArray name);
    void messageReceived(SpreadMessage message);
    void fatalError(QString message);

private:
    volatile bool running;
    int mailbox;
    void multicast(QByteArray groups, std::function<void (QByteArray)> func);
};

#endif // SPREADWORKER_H
