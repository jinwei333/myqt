#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QtNetwork>
#include "socketThread.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    MyServer(QObject *parent = Q_NULLPTR) :
        QTcpServer(parent)
    {

    }

protected:
    void incomingConnection(qintptr handle) override;

};

#endif // MYSERVER_H
