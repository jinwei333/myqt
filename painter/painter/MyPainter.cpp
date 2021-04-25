#include "MyPainter.h"
#include <QDebug>

MyPainter::MyPainter(QWidget *parent) : QWidget(parent)
{

}


void MyPainter::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    //  设置窗口背景色
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGray);
    painter.drawRect(rect());

    // 画矩形
//    painter.setRenderHint(QPainter::Antialiasing, true);    //设置渲染,启动反锯齿
    painter.setPen(QPen(Qt::green, 1));
    painter.drawLine(100, 200, 300, 200);

    painter.setWindow(100, 100, 200, 200);
    painter.setPen(QPen(Qt::blue, 1));
    painter.drawLine(100, 100, 300, 100);
    painter.drawLine(100, 300, 300, 300);

    painter.setViewport(200, 200, 200, 200);
    painter.setPen(QPen(Qt::red, 1));
    painter.drawLine(100, 200, 300, 200);
    painter.drawRect(0, 0, 100, 100);

    qDebug()<<"window的大小:"<<painter.window().width()<<" "<<painter.window().width();
    qDebug()<<"viewport的大小:"<<painter.viewport().width()<<" "<<painter.viewport().width();
}
