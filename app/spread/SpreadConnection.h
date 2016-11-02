#ifndef SPREADCONNECTION_H
#define SPREADCONNECTION_H

#include "SpreadWorker.h"
#include "SpreadGroup.h"
#include <QMutex>
#include <QString>
#include <vector>
#include <memory>
#include <utility>

class SpreadConnection
{
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

    typedef std::vector<SpreadGroupPtr> SpreadGroupList;

private:
    SpreadWorker worker;
    QMutex mutex;
    SpreadGroupList groups;
    QString hostname;
    int mailbox;
    int lastError;
    bool connected;

public:
    static QString getVersion();

    SpreadConnection();
    SpreadConnection(QByteArray user, QByteArray host, int port);
    ~SpreadConnection();

    // General API
    bool connect(QByteArray user, QByteArray host, int port);
    void disconnect();
    bool isConnected() const;
    QString getHostname() const;
    int getLastError() const;

    // Group API
    const SpreadGroupList& getGroups() const;
    SpreadGroup* joinGroup(QByteArray group);
    bool inGroup(QByteArray name) const;
    void leaveGroup(const SpreadGroup* group);

    // Message API
    bool sendMessage(QByteArray group, QByteArray message);
    const SpreadWorker* getWorker();
};

typedef std::unique_ptr<SpreadConnection> SpreadConnPtr;

#endif // SPREADCONNECTION_H
