#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->pushButton->installEventFilter(&filter);

    // 按钮1 将被过滤
    connect(ui->pushButton, &QPushButton::pressed, [](){
        qDebug() << "按钮1 按下";
    });
    // 按钮2 未安装事件过滤器
    connect(ui->pushButton2, &QPushButton::pressed, [](){
        qDebug() << "按钮2 按下";
    });
}

Widget::~Widget()
{
    delete ui;
}


bool MyFilter::eventFilter(QObject *watched, QEvent *ev)
{
    if(ev->type() == QEvent::MouseButtonPress || ev->type() == QEvent::MouseButtonRelease
            || ev->type() == QEvent::MouseButtonDblClick) {
        qDebug() << "sorry everybody, I gonna filter all the mouse event!";
        return true;
    }

    return QObject::eventFilter(watched, ev);
}
