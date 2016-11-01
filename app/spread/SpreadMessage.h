#ifndef SPREADMESSAGE_H
#define SPREADMESSAGE_H

#include <QByteArray>

struct SpreadMessage
{
    QByteArray group;
    QByteArray user;
    QByteArray text;

    SpreadMessage();
    SpreadMessage(QByteArray group, QByteArray user, QByteArray text);
};

#endif // SPREADMESSAGE_H
