#include "SpreadWorker.h"
#include <QCoreApplication>
#include <QMutexLocker>
#include <cassert>
#include <sp.h>

static QByteArray copyUntil(QByteArray bytes, char ch)
{
    int lastChar = bytes.indexOf(ch);
    if (lastChar < 0) {
        return bytes;
    }
    else {
        return {bytes.data(), lastChar};
    }
}

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
                if (status < 0 && status != GROUPS_TOO_SHORT && status != BUFFER_TOO_SHORT) {
                    emit fatalError("Erro interno: SP_receive() < 0 na primeira tentativa");
                }
                // Define os tamanhos máximos a partir da mensagem anterior
                int msgSize = -mismatch;
                int maxGroups = -numGroups;
                groups.resize(maxGroups * MAX_GROUP_NAME);
                message.resize(msgSize);
                // Trata mensagem de entrada ou saída do grupo
                if (Is_membership_mess(svcType)) {
                    QByteArray user = {MAX_GROUP_NAME, '\0'};
                    membership_info mbInfo;
                    // Algum membro entrou ou saiu de um grupo
                    if (Is_reg_memb_mess(svcType)) {
                        status = SP_receive (
                            mailbox, &svcType, sender.data(), maxGroups, &numGroups,
                            reinterpret_cast<char(*)[32]>(groups.data()), &msgType, &mismatch, msgSize, message.data()
                        );
                        if (status < 0) {
                            emit fatalError("Erro interno: SP_receive() < 0 em Is_membership_mess()");
                        }
                        status = SP_get_memb_info(message.data(), svcType, &mbInfo);
                        if (status < 0) {
                            emit fatalError("Erro interno: SP_get_memb_info() < 0 em Is_membership_mess()");
                        }
                        // Preenche o nome do usuário que entrou ou saiu do grupo
                        const char* member = mbInfo.changed_member;
                        qCopy(member, member + MAX_GROUP_NAME, user.begin());
                        sender = copyUntil(sender, '\0');
                        user = copyUntil(user, '\0');
                        if (Is_caused_join_mess(svcType)) {
                            emit userJoined(sender, user);
                        }
                        else {
                            emit userLeft(sender, user);
                        }
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
                        emit fatalError("Erro interno: SP_receive < 0 em Is_regular_mess()");
                        break;
                    }
                    multicast(groups, [this, sender, &message](QByteArray group) {
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
    // Verifica o tamanho do array de grupos
    if ((groups.size() % 32) != 0) {
        emit fatalError("Erro interno: (groups.size() % 32) != 0");
    }

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
