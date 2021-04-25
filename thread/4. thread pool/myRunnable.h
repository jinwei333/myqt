#ifndef MYRUNNABLE_H
#define MYRUNNABLE_H

#include <QRunnable>
#include <QString>
#include <QDebug>

class MyRunnable : public QRunnable
{
public:
    MyRunnable(const QString &threadName) :m_threadName(threadName)
    {

    }

    ~MyRunnable() {
        qDebug() << m_threadName << " ~MyRunnable";
    }
protected:
    void run();

private:
    QString m_threadName;
};

#endif // MYRUNNABLE_H
