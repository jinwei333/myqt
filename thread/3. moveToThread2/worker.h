#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();
    bool isStop() const { return m_Stop; }

public slots:
    void doWork();
    void setStop(bool flag);

signals:
    void setValue1(int);
    void workFinished();

private:
    bool m_Stop;
};

#endif // WORKER_H
