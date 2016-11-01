#include "SpreadGroup.h"
#include "SpreadConnection.h"

SpreadGroup::SpreadGroup(SpreadConnection* conn, QByteArray name)
    : connection(conn)
    , name(name)
{}

void SpreadGroup::sendMessage(QByteArray msg)
{
    connection->sendMessage(getName(), msg);
}

SpreadConnection* SpreadGroup::getConnection() const
{
    return connection;
}

QByteArray SpreadGroup::getName() const
{
    return name;
}
