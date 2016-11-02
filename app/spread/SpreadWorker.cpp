#include "SpreadWorker.h"
#include <QCoreApplication>
#include <sp.h>

SpreadWorker::SpreadWorker(QObject* parent)
    : QThread(parent)
    , running(false)
    , mailbox(-1)
{}

SpreadWorker::~SpreadWorker()
{}

void SpreadWorker::run()
{
    running = true;
    while (running) {
        if (mailbox >= 0) {
            int bytesLeft = SP_poll(mailbox);
            while (bytesLeft > 0) {
                // Variáveis pré-alocadas
                QByteArray sender = {MAX_GROUP_NAME, '\0'};
                QByteArray groups = {};
                QByteArray message = {};
                service svcType = 0;
                int16_t msgType = 0;
                int mismatch = 0;
                int numGroups;
                int status;
                // Verifica o tamanho necessário para o array de grupos
                status = SP_receive (
                    mailbox, &svcType, sender.data(), 0, &numGroups,
                    nullptr, &msgType, &mismatch, 0, nullptr
                );
                if (status != GROUPS_TOO_SHORT) {
                    emit fatalError("Número de grupos da não informado!");
                    return;
                }
                int maxNumGroups = -numGroups;
                groups.resize(maxNumGroups * MAX_GROUP_NAME);
                // Verifica o tamanho do buffer necessário para a mensagem
                status = SP_receive (
                    mailbox, &svcType, sender.data(), maxNumGroups, &numGroups,
                    reinterpret_cast<char(*)[32]>(groups.data()), &msgType, &mismatch, 0, nullptr
                );
                if (status != BUFFER_TOO_SHORT) {
                    emit fatalError("Tamanho do buffer da mensagem não informado!");
                    return;
                }
                int msgSize = -mismatch;
                message.resize(msgSize);
                // E finalmente, grava a mensagem
                status = SP_receive (
                    mailbox, &svcType, sender.data(), maxNumGroups, &numGroups,
                    reinterpret_cast<char(*)[32]>(groups.data()), &msgType, &mismatch, msgSize, message.data()
                );
                if (status < 0) {
                    emit fatalError("Erro ocorrido no recebimento da mensagem, servidor fora do ar?");
                    return;
                }
                QByteArray firstGroup = groups.mid(0, MAX_GROUP_NAME);
                emit messageReceived(SpreadMessage(firstGroup, sender, message));
                bytesLeft = SP_poll(mailbox);
            }
        }
        QCoreApplication::processEvents();
        msleep(100);
    }
}

void SpreadWorker::finish()
{
    running = false;
}
