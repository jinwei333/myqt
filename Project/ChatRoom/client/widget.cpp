#include "widget.h"
#include "ui_widget.h"
#include <QHostInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->portSpinBox->setRange(5000, 65535);

    QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());

    QList<QHostAddress> addrList = hostInfo.addresses();

    for(auto &ipAddr : addrList) {
        ui->IPcomboBox->addItem(ipAddr.toString());
    }
    if(ui->IPcomboBox->count())
        ui->IPcomboBox->setCurrentIndex(ui->IPcomboBox->count()-1);

    QString ipStr = ui->IPcomboBox->currentText();

    m_Client = new Client(ui->portSpinBox->value(), ipStr, this);
}

Widget::~Widget()
{
    if(m_Client->isRunning()) {
        m_Client->quit();
        m_Client->wait();
    }
    delete ui;
}


void Widget::on_loginBtn_clicked()
{
    m_Client->setPort(ui->portSpinBox->value());
    m_Client->setIPAddress(ui->IPcomboBox->currentText());

    if(m_Client->startClient()) {
        m_Client->start();
        ui->loginBtn->setDisabled(true);
        ui->logoutBtn->setEnabled(true);
    }
    else {
        ui->loginBtn->setEnabled(true);
        ui->logoutBtn->setEnabled(true);
        ui->logTextEdit->append("Connect failed!");
    }
}

void Widget::on_logoutBtn_clicked()
{
    if(m_Client->isRunning()) {
        m_Client->quit();
        m_Client->wait();
    }

    m_Client->stopClient();

    ui->loginBtn->setEnabled(true);
    ui->logoutBtn->setEnabled(false);
}

void Widget::on_sendBtn_clicked()
{
    QString text = ui->sendLineEdit->text();
    m_Client->sendData(text);
}
