#include "widget.h"
#include "ui_widget.h"
#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("Broadcast receiver");

    m_udpSocket = new QUdpSocket(this);

    m_udpSocket->bind(60000, QUdpSocket::ShareAddress);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &Widget::onReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onReadyRead()
{
    while(m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram(m_udpSocket->readAll());

        quint64 size = m_udpSocket->pendingDatagramSize();
        datagram.resize(size);

        quint16 port;
        QHostAddress host;

        if(m_udpSocket->readDatagram(datagram.data(), size, &host, &port) != -1)
            ui->textEdit->append(QString("[%1:%2] ").arg(host.toString()).arg(port)+datagram);
    }
}
