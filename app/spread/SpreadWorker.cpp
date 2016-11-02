#include "SpreadWorker.h"
#include <QCoreApplication>
#include <QMutexLocker>
#include <sp.h>

SpreadWorker::SpreadWorker(QMutex& mutex, QObject* parent)
    : QThread(parent)
    , running(false)
    , mailbox(-1)
    , mutex(mutex)
{}

SpreadWorker::~SpreadWorker()
{}

void SpreadWorker::run()
{
    running = true;
    while (running) {
        if (mailbox >= 0) {
            QMutexLocker locker(&mutex);
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
                // Verifica o tamanho necessário para o array de grupos e de tamanho de mensagem
                status = SP_receive (
                    mailbox, &svcType, sender.data(), 0, &numGroups,
                    nullptr, &msgType, &mismatch, 0, nullptr
                );
                int msgSize = -mismatch;
                int maxGroups = -numGroups;
                groups.resize(maxGroups * MAX_GROUP_NAME);
                message.resize(msgSize);
                // Trata mensagem de participação de grupo
                if (Is_membership_mess(svcType)) {
                    status = SP_receive (
                        mailbox, &svcType, sender.data(), maxGroups, &numGroups,
                        reinterpret_cast<char(*)[32]>(groups.data()), &msgType, &mismatch, msgSize, message.data()
                    );
                }
                // Trata mensagem de saída de grupo
                else if (Is_self_leave(svcType)) {
                    status = SP_receive (
                        mailbox, &svcType, sender.data(), maxGroups, &numGroups,
                        reinterpret_cast<char(*)[32]>(groups.data()), &msgType, &mismatch, msgSize, message.data()
                    );
                }
                // Trata mensagem comum
                else if (Is_regular_mess(svcType)) {
                    // Grava a mensagem no cliente
                    status = SP_receive (
                        mailbox, &svcType, sender.data(), maxGroups, &numGroups,
                        reinterpret_cast<char(*)[32]>(groups.data()), &msgType, &mismatch, msgSize, message.data()
                    );
                    if (status < 0) {
                        emit fatalError("Erro ocorrido no recebimento da mensagem, servidor fora do ar?");
                        break;
                    }
                    // Pára de ler o nome do grupo em MAX_GROUPSIZE (32) ou no primeiro byte nulo
                    QByteArray firstGroup = groups.mid(0, qMin(qstrlen(groups.data()), unsigned(MAX_GROUP_NAME)));
                    emit messageReceived(SpreadMessage(firstGroup, sender, message));
                }
                else {
                    emit fatalError("Não foi possível tratar o tipo de mensagem: (int: 0x" + QString::number(svcType, 16) + ")");
                }
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
