#include "SpreadGroup.h"
#include "SpreadConnection.h"

SpreadGroup::SpreadGroup(SpreadConnection* conn, const char* name)
    : connection(conn)
    , name(name)
{}

SpreadConnection* SpreadGroup::getConnection() const
{
    return connection;
}

std::string SpreadGroup::getName() const
{
    return name;
}
