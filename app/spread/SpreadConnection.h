#ifndef SPREADCONNECTION_H
#define SPREADCONNECTION_H
#include <string>
#include <vector>

class SpreadConnection
{
private:
    std::vector<std::string> groups;

public:
    static std::string getVersion();

    SpreadConnection(const char* host, int port);

    // Group API
    const std::vector<std::string>& getGroups() const;
    void joinGroup(const char* group);
    void leaveGroup(const char* group);

    // Message API
};

#endif // SPREADCONNECTION_H
