#include "button.h"
#include <QDebug>
#include <QApplication>
/**
    event->accept() 和 event->ignore() 一般不用，

    QEvent 默认是 accept，不向父组件传播事件
**/

const QEvent::Type MyEvent::type = (QEvent::Type)QEvent::registerEventType();


void Button::mousePressEvent(QMouseEvent *ev)
{
//    QPushButton::mousePressEvent(ev);   // 默认是 accept，事件不会传递到父对象
    qDebug() << "Button1::mousePressEvent";
    ev->ignore();
}

bool Button::event(QEvent *ev)
{
    if(ev->type() == QEvent::MouseButtonPress) {
        qDebug() << "Button1::event" << ev->type();
//        QPushButton::event(ev);
//        return false;
    }

    return QPushButton::event(ev);
}
