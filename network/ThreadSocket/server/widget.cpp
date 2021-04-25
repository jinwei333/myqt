#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_server(new MyServer(this))
{
    ui->setupUi(this);

    setWindowTitle(tr("Socket thread"));

    ui->portLineEdit->setReadOnly(true);
    ui->portLineEdit->setValidator(new QIntValidator(0, 65536, this));

    QList<QHostAddress> hostAddrList = QNetworkInterface::allAddresses();


    foreach (const QHostAddress& hostAddr, hostAddrList) {
        if(hostAddr!=QHostAddress::LocalHost && hostAddr.toIPv4Address()) {
            ui->addrComboBox->addItem(hostAddr.toString());
        }
    }
    ui->portLineEdit->setText(QString::number(60000));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_startBtn_clicked()
{
    QString ipAddr = ui->addrComboBox->currentText();

    if(ipAddr.isEmpty()) {
        return;
    }

    if(m_server->listen(QHostAddress(ipAddr), 60000) != true) {
        QMessageBox::critical(this, tr("Threaded Fortune Server"),
                                        tr("Unable to start the server: %1.")
                                        .arg(m_server->errorString()));
        return;
    }
    ui->startBtn->setEnabled(false);
}
