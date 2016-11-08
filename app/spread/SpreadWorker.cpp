#include "SpreadWorker.h"
#include <QCoreApplication>
#include <QMutexLocker>
#include <cassert>
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
                // Verifica o tamanho necessário para o array de grupos e de tamanho de mensagem
                status = SP_receive (
                    mailbox, &svcType, sender.data(), 0, &numGroups,
                    nullptr, &msgType, &mismatch, 0, nullptr
                );
                // Define os tamanhos máximos a partir da mensagem anterior
                int msgSize = -mismatch;
                int maxGroups = -numGroups;
                groups.resize(maxGroups * MAX_GROUP_NAME);
                message.resize(msgSize);
                // Trata mensagem de entrada ou saída do grupo
                if (Is_membership_mess(svcType)) {
                    membership_info mbInfo;
                    // Algum membro entrou ou saiu de um grupo
                    if (Is_reg_memb_mess(svcType)) {
                        status = SP_receive (
                            mailbox, &svcType, sender.data(), maxGroups, &numGroups,
                            reinterpret_cast<char(*)[32]>(groups.data()), &msgType, &mismatch, msgSize, message.data()
                        );

                        QByteArray firstGroup = groups.mid(0, qMin(qstrlen(groups.data()), unsigned(MAX_GROUP_NAME)));
                        emit messageReceived(SpreadMessage(firstGroup, sender, message));
                    }
                    // Evento de transição
                    else if (Is_transition_mess(svcType)) {
                        QByteArray firstGroup = groups.mid(0, qMin(qstrlen(groups.data()), unsigned(MAX_GROUP_NAME)));
                        emit messageReceived(SpreadMessage(firstGroup, sender, message));
                    }
                }
                // Trata mensagem de dados comum
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
                    multicast(groups, [this, sender, message](QByteArray group) {
                        emit messageReceived(SpreadMessage(group, sender, message));
                    });
                }
                else {
                    emit fatalError("Não foi possível tratar o tipo de mensagem: (int: 0x" + QString::number(svcType, 16) + ")");
                    break;
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

void SpreadWorker::multicast(QByteArray groups, std::function<void (QByteArray)> func)
{
    assert((groups.size() % 32) == 0);
    // Separa os nomes dos grupos a cada 32 caracteres (MAX_GROUP_NAME)
    const char* iterator = groups.constData();
    while (*iterator) {
        // Calcula o tamanho da string (menor valor entre MAX_GROUP_NAME e distância até o primeiro \0)
        // E então copia a string para um array contendo um grupo de destino da mensagem
        size_t strSize = qMin(strlen(iterator), size_t(MAX_GROUP_NAME));
        QByteArray group = {iterator, int(strSize)};
        func(group);
        iterator += MAX_GROUP_NAME;
    }
}
