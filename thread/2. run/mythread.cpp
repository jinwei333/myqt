#include "mythread.h"
#include <QDebug>


MyThread::MyThread(QObject *parent) : QThread(parent), m_count(0), m_Stop(false)
{
    connect(this, MyThread::started, [&](){
        m_count = 0;
        m_Stop = false;
    });
}

MyThread::~MyThread()
{
    qDebug() << "~MyThread";
}

void MyThread::run()
{
    qDebug() << "Counter thread id: " << QThread::currentThreadId();
    while(true)
    {
//        mutex.lock();
        if(m_Stop == true) {
            break;
        }
//        mutex.unlock();

        emit sendRunCount(m_count);
        msleep(50);
        m_count++;

        if(m_count > 100)
            break;
    }
}

void MyThread::onThreadStop()
{
//    mutex.lock();
    m_Stop = true;
//    mutex.unlock();
}

