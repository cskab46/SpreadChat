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
