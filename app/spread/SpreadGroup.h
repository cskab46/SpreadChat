#ifndef SPREADGROUP_H
#define SPREADGROUP_H

#include <QByteArray>
#include <string>
#include <memory>
class SpreadConnection;

class SpreadGroup
{
private:
    SpreadConnection* connection;
    std::string name;

public:
    SpreadGroup(SpreadConnection* conn, std::string name);
    void sendMessage(QByteArray msg);
    SpreadConnection* getConnection() const;
    std::string getName() const;
};

#endif // SPREADGROUP_H
