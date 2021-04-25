#include "button2.h"


bool Button2::event(QEvent *ev)
{
#if 1
    if(ev->type() == QEvent::MouseButtonPress) {
        qDebug() << "Button2::event" << ev->type();
        Button::event(ev);
        return true;
    }
#endif
    return Button::event(ev);
}

void Button2::mousePressEvent(QMouseEvent *ev)
{
//    QPushButton::mousePressEvent(ev);   // 默认是 accept，事件不会传递到父对象

    qDebug() << "Button2::mousePressEvent";
    ev->ignore();
}
