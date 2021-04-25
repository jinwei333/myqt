#ifndef BUTTONCHILD_H
#define BUTTONCHILD_H

#include <QObject>
#include "button.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QEvent>
#include <QDebug>

class ButtonChild : public QPushButton
{
public:
    explicit ButtonChild(QWidget *parent = nullptr) : QPushButton(parent)
    {
        setObjectName("Button2");
        connect(this, &Button::pressed, [](){
            qDebug() << "Button::pressed 槽函数";
        });
    }
protected:
    bool event(QEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);
signals:

};

#endif // BUTTONCHILD_H
