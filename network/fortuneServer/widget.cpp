#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDebug>

#define LISTEN_PORT 60000

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_server(Q_NULLPTR)
    , networkSession(0)
    , m_tcpSocket(Q_NULLPTR)
{
    ui->setupUi(this);

    setWindowTitle("Server");

    ui->portSpinBox->setValue(60000);

    // find out IP addresses of this machine
    QList<QHostAddress> hostAddrList = QNetworkInterface::allAddresses();

    foreach (const QHostAddress& hostAddr, hostAddrList) {
        if(hostAddr!=QHostAddress::LocalHost && hostAddr.toIPv4Address()) {
            ui->addrComboBox->addItem(hostAddr.toString());
        }
    }
    ui->addrComboBox->setCurrentIndex(ui->addrComboBox->count()-1);

    m_server = new QTcpServer(this);

    // 每当客户端连接时会发出 newConnection 信号。
    connect(m_server, &QTcpServer::newConnection, this, &Widget::newConnectionRequest);
}

Widget::~Widget()
{
    if(m_tcpSocket)
        m_tcpSocket->close();

    delete ui;
}

bool Widget::startListen()
{
    QString ipAddress = ui->addrComboBox->currentText() ;

    // if we did not find one, use IPv4 localhost
    if(ipAddress.isEmpty()) {
        QMessageBox::critical(this, "Listen", "Empty ip address");
        return false;
    }

    if(m_server->listen(QHostAddress(ipAddress), ui->portSpinBox->value()) != true) {
        QMessageBox::critical(this, "Server", tr("Unable to start the server. %1").arg(m_server->errorString()), QMessageBox::Cancel);
        return false;
    }

    return true;
}


void Widget::newConnectionRequest()
{    
    if(m_tcpSocket) {
        m_tcpSocket->close();
        m_tcpSocket = nullptr;
    }

//    关键代码
//    调用 nextPendingConnection 接受挂起的连接作为已连接的QTcpSocket。
    m_tcpSocket = m_server->nextPendingConnection();

    m_socketStream.setDevice(m_tcpSocket);
    m_socketStream.setVersion(QDataStream::Qt_4_0);

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &Widget::onReadyRead);

    // 客户端断开连接，QTcpSocket 自我销毁
    connect(m_tcpSocket, &QAbstractSocket::disconnected, m_tcpSocket, &QObject::deleteLater);
    connect(m_tcpSocket, &QAbstractSocket::destroyed, [&](){
        ui->textEdit->append("QTcpSocket::destroyed");
        m_tcpSocket = nullptr;
    });
    connect(m_tcpSocket, &QAbstractSocket::disconnected, [=](){
        ui->textEdit->append("QTcpSocket::disconnected");
    });
    ui->textEdit->append("A connection is established.");
}

void Widget::on_sendBtn_clicked()
{
    if(!m_tcpSocket) {
        QMessageBox::critical(this, "Server", tr("No socket connection"), QMessageBox::Cancel);
        return;
    }

    if(m_tcpSocket->isValid() != true) {
        QMessageBox::critical(this, "Server", tr("Socket is not valid or not ready for use"), QMessageBox::Cancel);
        return;
    }

    m_socketStream << ui->dataLineEdit->text();
}

void Widget::on_startBtn_clicked()
{
    if(true == startListen()) {
        ui->startBtn->setEnabled(false);
        ui->stopBtn->setEnabled(true);

        ui->textEdit->append(QString("Listen address: %1, port:%2").arg(m_server->serverAddress().toString()).arg(m_server->serverPort()));
    }
}

void Widget::on_stopBtn_clicked()
{
    if(m_server->isListening()) {

        // 关闭套接字的I/O设备，并调用disconnectFromHost()来关闭套接字的连接。
        if(m_tcpSocket) {
            m_tcpSocket->close();
        }
        m_server->close();
        ui->startBtn->setEnabled(true);
        ui->stopBtn->setEnabled(false);
        QMessageBox::information(this, "Server", tr("Server closed"));
    }
}

void Widget::onReadyRead()
{
    QString text;

//    QByteArray text = m_tcpSocket->readAll();

    m_socketStream >> text;
    ui->textEdit->append("Receive from client: " + text);
}
