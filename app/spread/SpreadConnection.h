#ifndef SPREADCONNECTION_H
#define SPREADCONNECTION_H
#include <string>
#include <vector>
#include <memory>
#include <utility>

class SpreadConnection
{
private:
    std::vector<std::string> groups;
    bool connected;

public:
    static std::string getVersion();

    SpreadConnection(const char* host, int port);

    // General API
    bool isConnected() const;

    // Group API
    const std::vector<std::string>& getGroups() const;
    void joinGroup(const char* group);
    void leaveGroup(const char* group);

    // Message API
};

typedef std::unique_ptr<SpreadConnection> SpreadConnPtr;

#endif // SPREADCONNECTION_H
