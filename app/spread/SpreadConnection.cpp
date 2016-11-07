#include "SpreadConnection.h"
#include "SpreadWorker.h"
#include <QMutexLocker>
#include <QDebug>
#include <algorithm>
#include <sp.h>

QString SpreadConnection::getVersion()
{
    int major, minor, patch;
    SP_version(&major, &minor, &patch);
    return QString("%1.%2.%3").arg(major).arg(minor).arg(patch);
}

SpreadConnection::SpreadConnection()
    : worker(mutex)
    , connected(false)
{}

SpreadConnection::SpreadConnection(QByteArray user, QByteArray host, int port)
    : worker(mutex)
{
    connect(user, host, port);
}

SpreadConnection::~SpreadConnection()
{
    disconnect();
}

bool SpreadConnection::connect(QByteArray user, QByteArray host, int port)
{
    disconnect();
    QByteArray address = QByteArray::number(port) + "@" + host;
    sp_time timeout;
    timeout.sec = 10;
    timeout.usec = 0;
    char group[MAX_GROUP_NAME];
    QMutexLocker locker(&mutex);
    int status = SP_connect_timeout(address.data(), user.data(), 0, 1, &mailbox, group, timeout);
    connected = (status == ACCEPT_SESSION);
    if (!connected) {
        lastError = -status;
        return false;
    }
    hostname = QString("%1:%2").arg(host.data()).arg(port);
    worker.mailbox = mailbox;
    worker.start();
    return true;
}

void SpreadConnection::disconnect()
{
    if (connected) {
        worker.finish();
        worker.wait(500);
        // Acaba com o trabalhador se ele não responder devido ao bug de design do spread receber mensagens do próprio cliente sincronamente na saída (entrando em deadlock)
        if (worker.isRunning()) {
            qDebug() << "INFO: WorkerThread parou de responder e será terminada...";
            worker.terminate();
            qDebug() << "INFO: WorkerThread finalizada com sucesso!";
        }
        worker.mailbox = -1;
        SP_disconnect(mailbox);
        connected = false;
        hostname = "";
    }
}

bool SpreadConnection::isConnected() const
{
    return connected;
}

QString SpreadConnection::getHostname() const
{
    return hostname;
}

int SpreadConnection::getLastError() const
{
    return lastError;
}

const SpreadConnection::SpreadGroupList& SpreadConnection::getGroups() const
{
    return groups;
}

SpreadGroup* SpreadConnection::joinGroup(QByteArray group)
{
    mutex.lock();
    int status = SP_join(mailbox, group.data());
    mutex.unlock();
    if (status != 0) {
        lastError = status;
        return nullptr;
    }
    groups.push_back(std::make_unique<SpreadGroup>(this, group));
    return groups.back().get();
}

bool SpreadConnection::inGroup(QByteArray name) const
{
    auto result = std::find_if(groups.begin(), groups.end(), [&](const SpreadGroupPtr& val) {
        return val->getName() == name;
    });
    return result != groups.end();
}

void SpreadConnection::leaveGroup(const SpreadGroup* group)
{
    mutex.lock();
    SP_leave(mailbox, group->getName().data());
    mutex.unlock();
    auto iter = std::remove_if(groups.begin(), groups.end(), [&](const SpreadGroupPtr& val) {
        return val->getName() == group->getName();
    });
    groups.erase(iter, groups.end());
}

bool SpreadConnection::sendMessage(QByteArray group, QByteArray message)
{
    mutex.lock();
    int length = SP_multicast(mailbox, SAFE_MESS, group.data(), 0, message.size(), message.data());
    mutex.unlock();
    if (length < 0) {
        lastError = -length;
        return false;
    }
    return true;
}

const SpreadWorker* SpreadConnection::getWorker()
{
    return &worker;
}
