#include "widget.h"
#include "ui_widget.h"

#include <QKeyEvent>
#include <QApplication>
#include <QTime>
#include <QDebug>

/**
    QApplication::sendEvent  同步分发事件
    QApplication::postEvent  异步分发事件

**/


QEvent::Type Widget::myType = (QEvent::Type)QEvent::registerEventType();

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    for(int i = 0; i < 10; i++) {
        //postEvent传递的事件必须是通过new创建的
        QApplication::postEvent(this, new QEvent(myType));

        qDebug() << "事件" << i << "被分发";
    }
}

Widget::~Widget()
{
    delete ui;
}


// 同步事件分发
void Widget::sendKeyEvent(int key)
{
    QKeyEvent pressEvent(QEvent::KeyPress, key, Qt::NoModifier);
    QApplication::sendEvent(ui->textEdit, &pressEvent);

    QKeyEvent releaseEvent(QEvent::KeyRelease, key, Qt::NoModifier);
    QApplication::sendEvent(ui->textEdit, &releaseEvent);
}

void Widget::customEvent(QEvent *ev)
{
    if(ev->type() == myType) {
        qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << "收到事件";
        myDelay(100);
    }

    QWidget::customEvent(ev);
}

void Widget::myDelay(int msec)
{
    QTime start = QTime::currentTime();
    QTime end;

    do {
        end = QTime::currentTime();
    } while(start.msecsTo(end) <= msec);
}


/*!
 * \brief   Widget::Widget 使用sendEvent同步分发事件
 *          使用QPushButton模拟键盘的回删和向前删除按键
 */
void Widget::on_backBtn_clicked()
{
    QApplication::sendKeyEvent(Qt::Key_Backspace);
}

void Widget::on_deleteBtn_clicked()
{
    QApplication::sendKeyEvent(Qt::Key_Delete);
}

void Widget::on_enterBtn_clicked()
{
    QApplication::sendKeyEvent(Qt::Key_Enter);
}
