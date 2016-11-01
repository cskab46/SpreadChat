#include "SpreadMessage.h"

SpreadMessage::SpreadMessage()
{}

SpreadMessage::SpreadMessage(QByteArray group, QByteArray user, QByteArray msg)
    : group(group)
    , user(user)
    , text(msg)
{}
