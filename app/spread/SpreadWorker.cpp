#include "SpreadWorker.h"
#include <QDebug>

SpreadWorker::SpreadWorker(QObject* parent)
    : QThread(parent)
    , running(false)
{}

SpreadWorker::~SpreadWorker()
{}

void SpreadWorker::run()
{
    running = true;
    while (running) {
        qDebug() << "Working... Tid:" << QThread::currentThreadId();
        msleep(100);
    }
}

void SpreadWorker::finish()
{
    running = false;
}
