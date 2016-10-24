#include "SpreadConnection.h"
#include <sp.h>
#include <sstream>

std::string SpreadConnection::getVersion()
{
    std::stringstream ss;
    int major, minor, patch;
    SP_version(&major, &minor, &patch);
    ss << major << "." << minor << "." << patch;
    return ss.str();
}

SpreadConnection::SpreadConnection()
    : connected(false)
{}

SpreadConnection::SpreadConnection(const char* user, const char* host, int port)
{
    connect(user, host, port);
}

SpreadConnection::~SpreadConnection()
{
    SP_disconnect(mailbox);
}

bool SpreadConnection::connect(const char* user, const char* host, int port)
{
    disconnect();
    std::stringstream ss;
    ss << port << "@" << host;
    char group[MAX_GROUP_NAME];
    int status = SP_connect(ss.str().c_str(), user, 0, 1, &mailbox, group);
    connected = (status == ACCEPT_SESSION);
    if (connected) {
        ss.str("");
        ss << host << ":" << port;
        hostname = ss.str();
        return true;
    }
    else {
        lastError = -status;
        return false;
    }
}

void SpreadConnection::disconnect()
{
    if (connected) {
        SP_disconnect(mailbox);
        connected = false;
        hostname = {};
    }
}

bool SpreadConnection::isConnected() const
{
    return connected;
}

std::string SpreadConnection::getHostname() const
{
    return hostname;
}

int SpreadConnection::getLastError() const
{
    return lastError;
}
