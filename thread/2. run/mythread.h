#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Worker : public QObject
{
public:
    Worker(QObject* parent=nullptr) : QObject(parent) {}

public slots:

};

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);
    ~MyThread();

    void setCount(int cnt) {
        m_count = cnt;
    }
    int getCount() const { return m_count; }

signals:
    void sendRunCount(int);
protected:
    void run() override;

public slots:
    void onThreadStop();

private:
    // 计数
    int m_count = 0;

    bool m_Stop;

    QMutex mutex;
};

#endif // MYTHREAD_H
