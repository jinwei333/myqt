#include "myRunnable.h"
#include <QThread>


void MyRunnable::run()
{

    qDebug() << m_threadName << " thread id: " << QThread::currentThreadId();
    int count = 0;

    while (true) {
        if(count >= 10)
            break;

        qDebug() << m_threadName << " count = " << count++;
        QThread::msleep(300);
    }
}
