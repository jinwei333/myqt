#include "widget.h"
#include "ui_widget.h"
#include <QHostInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    qDebug() << "Widget Thread ID: " << GetCurrentThreadId();

    ui->setupUi(this);

    ui->logTextEdit->append("Log text");

    ui->startBtn->setEnabled(true);
    ui->closeBtn->setEnabled(false);

    ui->portSpinBox->setRange(5000, 65535);

    QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());

    QList<QHostAddress> addrList = hostInfo.addresses();

    for(auto &ipAddr : addrList) {
        ui->IPcomboBox->addItem(ipAddr.toString());
    }
    if(ui->IPcomboBox->count())
        ui->IPcomboBox->setCurrentIndex(ui->IPcomboBox->count()-1);

    QString ipStr = ui->IPcomboBox->currentText();

    m_Server = new CServer(ui->portSpinBox->value(), ipStr);
    m_AcceptThread = new QThread();
    m_Server->moveToThread(m_AcceptThread);

    connect(m_AcceptThread, &QThread::finished, m_Server, &CServer::deleteLater);
    connect(m_Server, &CServer::acceptEnded, this, &Widget::onAcceptEnded);
    connect(this, &Widget::startAcceptThread, m_Server, &CServer::acceptProc);
    m_AcceptThread->start();

    connect(m_Server, &CServer::sendLogMessage, this, &Widget::onLogMsgRecv);
}

Widget::~Widget()
{
    if(m_AcceptThread->isRunning()) {
        m_AcceptThread->terminate();
        m_AcceptThread->wait();
    }
    delete m_Server;

    delete ui;
}

void Widget::on_startBtn_clicked()
{
    QString ipStr = ui->IPcomboBox->currentText();

    m_Server->setPort(ui->portSpinBox->value());
    m_Server->setIPAddress(ipStr);

    if(m_Server->startServer() == true) {
        if(!m_AcceptThread->isRunning())
            m_AcceptThread->start();
        emit startAcceptThread();
        ui->startBtn->setEnabled(false);
        ui->closeBtn->setEnabled(true);
    }
}

void Widget::on_closeBtn_clicked()
{
    m_AcceptThread->terminate();
    m_AcceptThread->wait();

    m_Server->stopServer();

    ui->startBtn->setEnabled(true);
    ui->closeBtn->setEnabled(false);
}

void Widget::onAcceptEnded()
{
    m_Server->stopServer();
}

void Widget::onLogMsgRecv(QString text)
{
    ui->logTextEdit->append(text);
}
