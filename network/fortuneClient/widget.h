#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void readTcpData();
    void onConnected();
    void onDisonnected();
    void setConnection();
    void setDisconnect();
    void socketErrorDisplay(QAbstractSocket::SocketError socketError);
    void sendData();

private:
    Ui::Widget *ui;

    QDataStream m_socketStream;
    QTcpSocket *m_tcpSocket;

};
#endif // WIDGET_H
