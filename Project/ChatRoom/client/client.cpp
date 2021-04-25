#include "client.h"
#include <QDebug>

#define DEFAULT_BUFLEN 512

Client::Client(QObject *parent) : QThread(parent)
{

}

Client::Client(UINT port, QString ip, QObject *parent) :
    QThread(parent), m_Port(port), m_IPAddr(ip)
{

}

Client::~Client()
{
    stopClient();
}

void Client::clientProc()
{
    int iResult;
    char buffer[DEFAULT_BUFLEN];

    while (true)
    {
        iResult = recv(m_connectSocket, buffer, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            qDebug() << "Received from server: " << QString(buffer);
        }
        else if (iResult == 0) {
            qDebug() << "Connection closed";
            break;
        }
        else {
            qDebug() << "receive failed with error: " << WSAGetLastError();
        }
    }
}

BOOL Client::startClient()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        qDebug() << "WSAStartup failed with error: " << iResult;
        return FALSE;
    }
    m_connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_connectSocket == INVALID_SOCKET) {
        qDebug() << "socket function failed with error: " << WSAGetLastError();
        WSACleanup();
        return FALSE;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(m_IPAddr.toLatin1());
    clientService.sin_port = htons(m_Port);

    //----------------------
    // Connect to server.
    iResult = ::connect(m_connectSocket, (SOCKADDR *)&clientService, sizeof(clientService));
    if (iResult == SOCKET_ERROR) {
        qDebug() << "connect function failed with error: " << WSAGetLastError();
        if (closesocket(m_connectSocket) == SOCKET_ERROR)
            qDebug() << "closesocket function failed with error: " << WSAGetLastError();
        WSACleanup();
        return FALSE;
    }
    qDebug() << "Connected to server.";
    return TRUE;
}

void Client::stopClient()
{
    int iResult;
    if(m_connectSocket != INVALID_SOCKET) {
        iResult = closesocket(m_connectSocket);
        if (iResult == SOCKET_ERROR) {
            qDebug() << "closesocket function failed with error: " << WSAGetLastError();
        }
    }
    WSACleanup();
}

BOOL Client::sendData(QString &text)
{
    if(m_connectSocket == INVALID_SOCKET)
        return FALSE;

    int iResult;
    char *pData = text.toLatin1().data();
    //----------------------
    // Send an initial buffer
    iResult = send(m_connectSocket, pData, (int)strlen(pData), 0);
    if (iResult == SOCKET_ERROR) {
        qDebug() << "send failed with error: " << WSAGetLastError();
        return FALSE;
    }
    return true;
}

void Client::run()
{
    clientProc();
}
