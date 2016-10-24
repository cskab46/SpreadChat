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

SpreadConnection::SpreadConnection(const char* host, int port)
{
    connected = true;
}

bool SpreadConnection::isConnected() const
{
    return connected;
}
