#include "worker.h"
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent), m_isStop(false), m_isStop2(false)
{

}

Worker::~Worker()
{
    qDebug() << "~Worker";
}

void Worker::doWork1()
{
    m_isStop = false;
    qDebug() << "Worker1 Thread ID: " << QThread::currentThreadId();

    for(int i = 0; i < 100; i++) {
        emit setValue1(i + 1);
        QThread::msleep(20);

        if(m_isStop == true) {
            break;
        }
    }
    emit processFinished();
}

void Worker::doWork2()
{
    m_isStop2 = false;
    qDebug() << "Worker2 Thread ID: " << QThread::currentThreadId();

    for(int i = 0; i < 100; i++) {
        emit setValue2(i + 1);
        QThread::msleep(50);

        if(m_isStop2 == true) {
            break;
        }
    }
    emit processFinished();
}

