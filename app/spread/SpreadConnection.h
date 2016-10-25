#ifndef SPREADCONNECTION_H
#define SPREADCONNECTION_H

#include "SpreadWorker.h"
#include "SpreadGroup.h"
#include <string>
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

    typedef std::vector<std::unique_ptr<SpreadGroup>> SpreadGroupList;

private:
    SpreadWorker worker;
    SpreadGroupList groups;
    std::string hostname;
    int mailbox;
    int lastError;
    bool connected;

public:
    static std::string getVersion();

    SpreadConnection();
    SpreadConnection(const char* user, const char* host, int port);
    ~SpreadConnection();

    // General API
    bool connect(const char* user, const char* host, int port);
    void disconnect();
    bool isConnected() const;
    std::string getHostname() const;
    int getLastError() const;

    // Group API
    const SpreadGroupList& getGroups() const;
    const SpreadGroup* joinGroup(const char* group);
    bool inGroup(const char* name) const;
    void leaveGroup(const SpreadGroup* group);

    // Message API
};

typedef std::unique_ptr<SpreadConnection> SpreadConnPtr;

#endif // SPREADCONNECTION_H
