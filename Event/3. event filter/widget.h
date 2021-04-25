#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class MyFilter : public QObject
{
    Q_OBJECT
public:
    MyFilter() {}
protected:
    bool eventFilter(QObject *watched, QEvent *ev);

};

class MyApplaction : public QApplication {
    MyApplaction(int argc, char **argv) : QApplication(argc, argv) {}

    bool notify(QObject *obj, QEvent *ev)
    {
        if(ev->type() == QEvent::MouseButtonPress || ev->type() == QEvent::MouseButtonRelease
                || ev->type() == QEvent::MouseButtonDblClick) {
            qDebug() << "sorry everybody, I gonna filter all the mouse event!";
            return true;
        }
    }


};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    MyFilter filter;
};
#endif // WIDGET_H
