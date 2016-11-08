#include "SpreadConnection.h"

#include "SpreadWorker.h"
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
    : worker(WorkerGetters {this}, nullptr)
    , mailbox(-1)
    , connected(false)
{}

SpreadConnection::SpreadConnection(QByteArray user, QByteArray host, int port)
    : worker(WorkerGetters {this}, nullptr)
    , mailbox(-1)
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
    int status = SP_connect_timeout(address.data(), user.data(), 0, 1, &mailbox, group, timeout);
    connected = (status == ACCEPT_SESSION);
    if (!connected) {
        lastError = -status;
        return false;
    }
    hostname = QString("%1:%2").arg(host.data()).arg(port);
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
        SP_disconnect(mailbox);
        mailbox = -1;
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

bool SpreadConnection::joinGroup(QByteArray name)
{
    int status = SP_join(mailbox, name.data());
    if (status != 0) {
        lastError = status;
        return false;
    }
    groups.append(name);
    groupMembers.insert({name, {}});
    return true;
}

bool SpreadConnection::inGroup(QByteArray name) const
{
    return groups.contains(name);
}

void SpreadConnection::leaveGroup(QByteArray name)
{
    if (inGroup(name)) {
        SP_leave(mailbox, name.data());
        groups.removeAll(name);
        groupMembers.erase(groupMembers.find(name));
    }
}

QByteArrayList SpreadConnection::getUsers(QByteArray group) const
{
    return groupMembers.find(group)->second;
}

bool SpreadConnection::sendMessage(QByteArray group, QByteArray message)
{
    int length = SP_multicast(mailbox, SAFE_MESS, group.data(), 0, message.size(), message.data());
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
