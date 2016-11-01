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
    QByteArray name;

public:
    SpreadGroup(SpreadConnection* conn, QByteArray name);
    void sendMessage(QByteArray msg);
    SpreadConnection* getConnection() const;
    QByteArray getName() const;
};

typedef std::unique_ptr<SpreadGroup> SpreadGroupPtr;

#endif // SPREADGROUP_H
