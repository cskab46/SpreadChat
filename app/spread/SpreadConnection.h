#ifndef SPREADCONNECTION_H
#define SPREADCONNECTION_H

#include "SpreadWorker.h"
#include "WorkerGetters.h"
#include <QByteArrayList>
#include <QString>
#include <map>
#include <memory>
#include <utility>

class SpreadConnection
{
    friend class WorkerGetters;

public:
    enum {
        ILLEGAL_SPREAD = 1,
        COULD_NOT_CONNECT = 2,
        REJECT_QUOTA = 3,
        REJECT_NO_NAME = 4,
        REJECT_ILLEGAL_NAME = 5,
        REJECT_NOT_UNIQUE = 6,
        REJECT_VERSION = 7,
        CONNECTION_CLOSED = 8,
        REJECT_AUTH = 9
    };

    typedef QByteArrayList SpreadGroupList;
    typedef std::map<QByteArray, QByteArrayList> SpreadGroupMemberList;

private:
    SpreadWorker worker;
    SpreadGroupList groups;
    SpreadGroupMemberList groupMembers;
    QString hostname;
    int mailbox;
    int lastError;
    bool connected;

public:
    static QString getVersion();

    SpreadConnection();
    SpreadConnection(QByteArray user, QByteArray host, int port);
    SpreadConnection(const SpreadConnection&) = delete;
    ~SpreadConnection();

    // General API
    bool connect(QByteArray user, QByteArray host, int port);
    void disconnect();
    bool isConnected() const;
    QString getHostname() const;
    int getLastError() const;

    // Group API
    const SpreadGroupList& getGroups() const;
    bool joinGroup(QByteArray name);
    bool inGroup(QByteArray name) const;
    void leaveGroup(const QByteArray name);
    QByteArrayList getUsers(QByteArray group) const;

    // Message API
    bool sendMessage(QByteArray group, QByteArray message);
    const SpreadWorker* getWorker();
};

typedef std::unique_ptr<SpreadConnection> SpreadConnPtr;

#endif // SPREADCONNECTION_H
