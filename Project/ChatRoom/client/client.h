#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QThread>
#include <winsock2.h>

class Client : public QThread
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    Client(UINT port, QString ip, QObject *parent = nullptr);
    ~Client();
    BOOL startClient();
    void stopClient();
    BOOL sendData(QString &text);

    void setPort(UINT port) { m_Port = port; }
    void setIPAddress(QString ip) { m_IPAddr = ip; }

protected:
    virtual void run();

public slots:
    void clientProc(void);
signals:


private:
    UINT m_Port = 50000;
    QString m_IPAddr;
    SOCKET m_connectSocket = INVALID_SOCKET;
};

#endif // CLIENT_H
