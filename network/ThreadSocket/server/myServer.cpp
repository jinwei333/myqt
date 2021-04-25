#include "myServer.h"
#include <QDebug>

void MyServer::incomingConnection(qintptr handle)
{
    SocketThread *sockThread = new SocketThread(handle, this);
    connect(sockThread, &QThread::finished, sockThread, &QThread::deleteLater);
    sockThread->start();

    qDebug() << "Socket thread started";
}
