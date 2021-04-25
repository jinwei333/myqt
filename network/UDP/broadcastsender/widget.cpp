#include "widget.h"
#include "ui_widget.h"
#include <QTextEdit>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_updSocket = new QUdpSocket(this);

    setWindowTitle("Broadcast sender");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_sendBtn_clicked()
{
    QString text = ui->msgLineEdit->text();

    m_updSocket->writeDatagram(text.toLatin1().data(), text.size(), QHostAddress::Broadcast, 60000);
}
