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

    bool startListen();

private slots:
    void newConnectionRequest();

    void on_sendBtn_clicked();

    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void onReadyRead();
private:
    Ui::Widget *ui;

    QTcpServer *m_server;

    QNetworkSession *networkSession;

    QTcpSocket *m_tcpSocket;

    QDataStream m_socketStream;

};
#endif // WIDGET_H
