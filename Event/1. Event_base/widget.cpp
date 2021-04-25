#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->lineEdit->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Widget event 事件分发后: " << ev->type();
}


// 事件分发
// 返回值： true  -- 事件已被处理，不要再向父组件传播
//        false -- 向父组件传播
bool Widget::event(QEvent *ev)
{
    /// 自定义事件
    if(ev->type() == MyEvent::type) {
        MyEvent *myEvent = (MyEvent *)ev;

        qDebug() << "Widget::event " << ev->type() << myEvent->x << " " << myEvent->y << " "  << myEvent->z;

        return true;    // 不要向父组件（QWidget）传播
    }

    if(ev->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(ev);
        switch(keyEvent->key()) {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            qDebug() << "分发按键：" << keyEvent->text();
            break;
        default:
            qDebug() << "不分发的按键" << keyEvent->text();
            return true;
        }
    }

    //按照原来的流程来进行事件的分发
    if(ev->type() == QEvent::MouseButtonPress) {
        qDebug() << "Widget event 事件分发前: " << ev->type();
    }

    return QWidget::event(ev);
}

bool Widget::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ui->lineEdit && ev->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(ev);
        switch(keyEvent->key()) {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            qDebug() << keyEvent->text() << "is passed";
            return false;    //只有数字才会被传递到组件对象
        default:
            qDebug() << keyEvent->text() << "is filtered";
            return true; // 事件不再进行传播，拦截
        }
    }

    //当不确定是否继续传播时，按照父类的方法来处理，即调用父类的evenFilter函数
    return QWidget::eventFilter(obj, ev);
}

void Widget::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << "收到的分发按键：" << ev->text();
}

void Widget::customEvent(QEvent *ev)
{
    qDebug() << "customEvent " << ev->type();
}


void Widget::on_sendBtn_clicked()
{
    MyEvent myevent(1, 2, 3);

    bool ret = QApplication::sendEvent(this, &myevent);

    qDebug() << "自定义事件 sendEvent，返回值：" << ret;
}

void Widget::on_postBtn_clicked()
{
    MyEvent *myevent = new MyEvent(1, 2, 3);

    QApplication::postEvent(this, myevent);

    qDebug() << "自定义事件 postEvent";
}
