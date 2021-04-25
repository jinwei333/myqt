#include "cserver.h"
#include <winsock2.h>
#include <stdio.h>
#include <QDebug>

DWORD CServer::clientThreadProc(LPVOID pParam)
{
    int iResult;
    ClientItem *pClient = (ClientItem *)pParam;

    printf("Client %s connected.\n", pClient->m_szIpAddr);

    char buffer[DEFAULT_BUFLEN];
    while (true)
    {
        iResult = recv(pClient->m_Socket, buffer, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            qDebug() << "Received from client: " << QString(buffer);
        }
        else if (iResult == 0) {
            CServer().removeTargetClient(pClient);
            return 1;
        }
        else {
            int error = WSAGetLastError();
            qDebug() << "recv failed with error: " << error;
            if (error == WSAECONNRESET) {
                CServer().removeTargetClient(pClient);
                return 1;
            }
            else
                continue;
        }

        //----------------------
        // Send an initial buffer
        iResult = send(pClient->m_Socket, buffer, strlen(buffer) + 1, 0);
        if (iResult == SOCKET_ERROR) {
            qDebug() << "send failed with error: " << WSAGetLastError();
        }
    }

    return 0;
}

CServer::CServer(UINT port, QString ip, QObject *parent) :
    QObject(parent), m_Port(port), m_IP(ip), m_ListenSocket(INVALID_SOCKET)
{

}

CServer::~CServer()
{
    qDebug() << "~CServer" << endl;
    stopServer();
}

WINBOOL CServer::startServer()
{
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        qDebug() << "WSAStartup failed with error: " << iResult;
        emit sendLogMessage("WSAStartup failed with error: " + QString::number(iResult));
        return FALSE;
    }
    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ListenSocket == INVALID_SOCKET) {
        qDebug() << "socket failed with error: " << WSAGetLastError();
        emit sendLogMessage("socket failed with error: " + QString::number(WSAGetLastError()));
        WSACleanup();
        return FALSE;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;

    service.sin_addr.s_addr = inet_addr(m_IP.toLatin1());
    service.sin_port = htons(m_Port);

    if (bind(m_ListenSocket,
        (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR) {
        qDebug() << "bind failed with error: " << WSAGetLastError();
        emit sendLogMessage("bind failed with error: " + QString::number(WSAGetLastError()));
        closesocket(m_ListenSocket);
        WSACleanup();
        return FALSE;
    }
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(m_ListenSocket, 1) == SOCKET_ERROR) {
        qDebug() << "listen failed with error: " << WSAGetLastError();
        emit sendLogMessage("listen failed with error: " + QString::number(WSAGetLastError()));
        closesocket(m_ListenSocket);
        WSACleanup();
        return FALSE;
    }

    return TRUE;
}

void CServer::stopServer()
{
    if(m_ListenSocket != INVALID_SOCKET) {

        for(auto item : m_clientList)
        {
            removeTargetClient(item);
        }
        m_clientList.clear();
        // No longer need server socket
        if(closesocket(m_ListenSocket) == SOCKET_ERROR) {
            qDebug() << "closesocket failed with error: " << WSAGetLastError();
//            emit sendLogMessage("closesocket failed with error: " + QString::number(WSAGetLastError()));
        }
        m_ListenSocket = INVALID_SOCKET;
        if(WSACleanup() == SOCKET_ERROR) {
            qDebug() << "WSACleanup failed with error: " << WSAGetLastError();
//            emit sendLogMessage("WSACleanup failed with error: " + QString::number(WSAGetLastError()));
        }
        emit sendLogMessage("Server stopped!");
    }
}

void CServer::removeTargetClient(ClientItem *pItem)
{
    if(pItem == nullptr)
        return;

    closesocket(pItem->m_Socket);
    int pos = m_clientList.indexOf(pItem);
    if(pos == -1)
        return;

    m_clientList.removeAt(pos);

    delete pItem;

    qDebug() << QString(pItem->m_szIpAddr) << " connection closed" << endl;
    emit sendLogMessage(QString(pItem->m_szIpAddr) + " connection closed");
}

UINT CServer::acceptProc()
{
    DWORD threadId;

    if (m_ListenSocket == INVALID_SOCKET)
        return 1;

    qDebug() << "acceptProc Thread ID: " << GetCurrentThreadId();
    while (true)
    {
        sockaddr_in clientAddr;
        int iLen = sizeof(clientAddr);

        //----------------------
        // Create a SOCKET for accepting incoming requests.
        qDebug() << "Waiting for client to connect...\n";
        emit sendLogMessage("Waiting for client to connect...");
        SOCKET AcceptSocket = accept(m_ListenSocket, (sockaddr *)&clientAddr, &iLen);
        if (AcceptSocket == INVALID_SOCKET) {
            qDebug() << "accept failed with error: " << WSAGetLastError();
            emit sendLogMessage("accept failed with error: " + QString::number(WSAGetLastError()));
            continue;
        }
        ClientItem *pItem = new ClientItem();
        pItem->m_Socket = AcceptSocket;
        char *strIp = inet_ntoa(clientAddr.sin_addr);
        strncpy_s(pItem->m_szIpAddr, MAX_IP_LEN, strIp, MAX_IP_LEN);

        pItem->m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CServer::clientThreadProc, pItem, 0, &threadId);

        m_clientList.push_back(pItem);
    }

    return 0;
}
