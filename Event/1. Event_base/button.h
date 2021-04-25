#ifndef BUTTON_H
#define BUTTON_H


#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

/**
    自定义两个按钮类 Button 和 ButtonChild，重写 mousePressEvent，观察

    实现自定义一个事件类，按下按钮后发送事件到主窗口

**/

// 自定义一个事件类
class MyEvent : public QEvent
{
public:
    static const Type type;

    MyEvent() : QEvent(type) { }
    ~MyEvent() { qDebug() << "~MyEvent";  }
    MyEvent(int a, int b, int c) :QEvent(type), x(a), y(b), z(c) {

    }
    int x = 0;
    int y = 0;
    int z = 0;
};

class Button : public QPushButton
{
public:
    explicit Button(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setObjectName("Button1");
    }
protected:
    virtual void mousePressEvent(QMouseEvent *ev);

    bool event(QEvent *e);
signals:
};


#endif // BUTTON_H
