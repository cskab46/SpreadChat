#ifndef WORKERGETTERS_H
#define WORKERGETTERS_H

#include <QByteArrayList>
#include <QByteArray>

class SpreadConnection;

// Classe para quebrar o encapsulamento entre SpreadConnection e SpreadWorker de forma segura
class WorkerGetters {
public:
    WorkerGetters(SpreadConnection* owner);
    QByteArrayList& getMemberList(QByteArray group);
    int getMailbox() const;

private:
    SpreadConnection* owner;
};
#endif // WORKERGETTERS_H
