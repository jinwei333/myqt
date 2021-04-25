#include "socketThread.h"
#include <QDebug>

void SocketThread::run()
{
    QTcpSocket socket;

    if(true != socket.setSocketDescriptor(m_socketDescriptor)) {
        emit socketError(socket.error());
        return;
    }

    QDataStream sockStream(&socket);
    sockStream.setVersion(QDataStream::Qt_4_0);
    sockStream << QString("Hello, I am server");
    socket.disconnectFromHost();
    socket.waitForDisconnected();    

    qDebug() << "Socket thread: " << QThread::currentThreadId();
}
