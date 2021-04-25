#ifndef BUTTON2_H
#define BUTTON2_H

#include "button.h"
#include <QApplication>
#include <QEvent>

class Button2 : public Button
{
public:
    explicit Button2(QWidget *parent = nullptr) : Button(parent)
    {
        setObjectName("Button2");
    }
protected:
    bool event(QEvent *e);
    void mousePressEvent(QMouseEvent *ev);
signals:
};
#endif // BUTTON2_H
