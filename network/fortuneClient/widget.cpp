#include "widget.h"
#include "ui_widget.h"
#include "widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    ui->portSpinBox->setRange(0, 65535);
    ui->portSpinBox->setValue(60000);

    m_socketStream.setDevice(m_tcpSocket);
    m_socketStream.setVersion(QDataStream::Qt_4_0);

    // 设置验证器
//    ui->portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    // find out IP addresses of this machine
    QList<QHostAddress> hostAddrList = QNetworkInterface::allAddresses();

    foreach (const QHostAddress& hostAddr, hostAddrList) {
        if(hostAddr!=QHostAddress::LocalHost && hostAddr.toIPv4Address()) {
            ui->addrComboBox->addItem(hostAddr.toString());
        }
    }
    ui->addrComboBox->setCurrentIndex(ui->addrComboBox->count()-1);

    connect(ui->connectBtn, &QPushButton::clicked, this, &Widget::setConnection);
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &Widget::setDisconnect);
    connect(ui->sendBtn, &QPushButton::clicked, this, &Widget::sendData);

    connect(m_tcpSocket, &QIODevice::readyRead, this, &Widget::readTcpData);
    connect(m_tcpSocket, &QTcpSocket::connected, this, &Widget::onConnected);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &Widget::onDisonnected);
    connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Widget::socketErrorDisplay);

    setWindowTitle("Client");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::readTcpData()
{
    // 在流上启动一个新的读事务。
//    m_socketStream.startTransaction();
    QString text;
//    m_socketStream >> text;
//    if(!m_socketStream.commitTransaction())
//        return;

    QByteArray arr = m_tcpSocket->readAll();
#if 0
    QString hex = arr.toHex(':');
    qDebug() << "hex: " << hex;

    for (int i = 0; i < arr.size(); i++) {
        text += QString::number(arr.at(i), 16);
        text += ' ';
    }
#endif
    ui->textEdit->append("Receive from server: " + arr);
}

void Widget::onConnected()
{
    ui->connectBtn->setEnabled(false);
    ui->disconnectBtn->setEnabled(true);
    ui->textEdit->append("Connected from server");
}

void Widget::onDisonnected()
{
    ui->connectBtn->setEnabled(true);
    ui->disconnectBtn->setEnabled(false);
    ui->textEdit->append("Disonnected from server");
}

void Widget::setConnection()
{
    m_tcpSocket->abort();
    m_tcpSocket->connectToHost(ui->addrComboBox->currentText(),
                               ui->portSpinBox->value());

    ui->connectBtn->setEnabled(false);
    ui->disconnectBtn->setEnabled(true);
}

void Widget::setDisconnect()
{
//    m_tcpSocket->disconnectFromHost();

    // immediately closes the socket, discarding any pending data in the write buffer.
    m_tcpSocket->abort();
    ui->connectBtn->setEnabled(true);
    ui->disconnectBtn->setEnabled(false);
}

void Widget::socketErrorDisplay(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    // 远程主机关闭了连接
    case QAbstractSocket::RemoteHostClosedError:
        break;
    // 连接被拒绝(或超时)。
    case QAbstractSocket::ConnectionRefusedError:
        ui->connectBtn->setEnabled(true);
        QMessageBox::information(this, tr("Client"),
                                           tr("The connection was refused by the peer. "
                                              "Make sure the fortune server is running, "
                                              "and check that the host name and port "
                                              "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Client"),
                                           tr("The following error occurred: %1.")
                                           .arg(m_tcpSocket->errorString()));
    }
}

void Widget::sendData()
{
    QString text(ui->dataLineEdit->text());

    if(m_tcpSocket->state() != QAbstractSocket::ConnectedState) {
        QMessageBox::critical(this, "State", tr("The socket is not connected."));
        return;
    }

    m_socketStream << ui->dataLineEdit->text();
}

