#include "buttonchild.h"
#include <QDebug>
#include <QApplication>

bool ButtonChild::event(QEvent *ev)
{
    if(ev->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(ev);

        // 不是 左按钮 就不分发事件
        if(mouseEvent->button() != Qt::LeftButton) {
            qDebug() << "ButtonChild event 事件不分发";
            return true;
        }
        else {
            qDebug() << "ButtonChild event 事件分发";
        }
    }
    return QPushButton::event(ev);
}

void ButtonChild::mousePressEvent(QMouseEvent *ev)
{
    QPushButton::mousePressEvent(ev);

    // ignore 让事件继续传播，因此父组件 Widget 也收到了这个事件。
    ev->ignore();
    qDebug() << objectName() << ev->type() << "ignore 忽略事件处理，希望父对象来执行这个信号";
}
