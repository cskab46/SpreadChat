#ifndef SPREADGROUP_H
#define SPREADGROUP_H

#include <string>
#include <memory>
class SpreadConnection;

class SpreadGroup
{
private:
    SpreadConnection* connection;
    std::string name;

public:
    SpreadGroup(SpreadConnection* conn, const char* name);
    SpreadConnection* getConnection() const;
    std::string getName() const;
};

#endif // SPREADGROUP_H
