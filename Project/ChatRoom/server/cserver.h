#ifndef CSERVER_H
#define CSERVER_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QObject>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 27015

#define MAX_IP_LEN 16

// 连接一个客户端，开启一个线程
struct ClientItem
{
    SOCKET m_Socket;
    CHAR  m_szIpAddr[MAX_IP_LEN];
    HANDLE m_hThread;

    ClientItem() {
        m_Socket = INVALID_SOCKET;
        ZeroMemory(m_szIpAddr, MAX_IP_LEN);
        m_hThread = NULL;
    }
};

class CServer : public QObject
{
    Q_OBJECT
public:
    static DWORD clientThreadProc(LPVOID pParam);
public:
    CServer(QObject *parent = 0) : QObject(parent) { }
    CServer(UINT port, QString ip ,QObject *parent=0);
    ~CServer();
    WINBOOL startServer();

    void removeTargetClient(ClientItem *pItem);

    void setPort(UINT port) { m_Port = port; }
    void setIPAddress(QString ip) { m_IP = ip; }

    UINT   m_Port = 0;
    QString m_IP;
    SOCKET m_ListenSocket;

public slots:
    UINT acceptProc();
    void stopServer();
signals:
    void acceptEnded();
    void sendLogMessage(QString text);
private:
    QList<ClientItem *> m_clientList;
};

#endif // CSERVER_H
