#include "worker.h"
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent), m_Stop(false)
{

}

Worker::~Worker()
{
    qDebug() << "~Worker";
}

void Worker::doWork()
{
    qDebug() << "Worker1 thread id: " << QThread::currentThreadId();

    for(int i = 0; i < 100; i++) {
        emit setValue1(i + 1);
        QThread::msleep(20);

        if(isStop())
            break;
    }
    emit workFinished();
}

void Worker::setStop(bool flag)
{
    m_Stop = flag;
}


