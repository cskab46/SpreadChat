#include "WorkerGetters.h"

#include "SpreadConnection.h"

WorkerGetters::WorkerGetters(SpreadConnection* owner)
    : owner(owner)
{}

QByteArrayList& WorkerGetters::getMemberList(QByteArray group)
{
    return owner->groupMembers[group];
}

int WorkerGetters::getMailbox() const
{
    return owner->mailbox;
}
