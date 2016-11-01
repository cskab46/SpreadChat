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
            int numMessages = SP_poll(mailbox);
            while (numMessages > 0) {
                char sender[MAX_GROUP_NAME] = {};
                char groups[10][MAX_GROUP_NAME] = {{}};
                service svcType = 0;
                int16_t msgType = 0;
                int mismatch = 0;
                int groupNum;
                SP_receive(mailbox, &svcType, sender, 10, &groupNum, groups, &msgType, &mismatch, 0, nullptr);
                // O spread passa o tamanho do buffer necessário como o negativo do valor do endian mismatch
                QByteArray message(-mismatch, '\0');
                SP_receive(mailbox, &svcType, sender, 10, &groupNum, groups, &msgType, &mismatch, message.size(), message.data());
                emit messageReceived(SpreadMessage(groups[0], sender, message));
                numMessages = SP_poll(mailbox);
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
