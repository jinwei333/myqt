#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();
public slots:
    void doWork1();
    void doWork2();

    void setStop() {
        m_isStop = true;
        m_isStop2 = true;
    }
signals:
    void setValue1(int);
    void setValue2(int);
    void processFinished();

private:
    bool m_isStop = false;
    bool m_isStop2 = false;
};

#endif // WORKER_H
