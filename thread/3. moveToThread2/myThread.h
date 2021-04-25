#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QDebug>

class MyThread :public QThread
{
public:
    MyThread(QObject *parent = nullptr) : QThread(parent) {}

    ~MyThread() { qDebug() << "~MyThread"; }
protected:
    void run() {
        int ret = 0;
        qDebug() << "Enter run function" << QThread::currentThreadId();
        ret = QThread::exec();
        qDebug() << "Leave run function, ret = " << ret;
    }
};


#endif // MYTHREAD_H
