#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QObject>
#include <QThread>
#include <QtNetwork>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr socketDescriptor, QObject *parent = Q_NULLPTR) :
        QThread(parent)
        , m_socketDescriptor(socketDescriptor)
    {
    }

private slots:


signals:
    void socketError(QTcpSocket::SocketError err);
protected:
    void run() override;
private:
    qintptr m_socketDescriptor;
};

#endif // SOCKETTHREAD_H
