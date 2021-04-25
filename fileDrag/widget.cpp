#include "widget.h"
#include "ui_widget.h"
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setAcceptDrops(false);
}

Widget::~Widget()
{
    delete ui;
}

#if 0
void Widget::dragEnterEvent(QDragEnterEvent *e)
{
    qDebug() << "Widget::dragEnterEvent";
    e->acceptProposedAction();

    QWidget::dragEnterEvent(e);
}

void Widget::dropEvent(QDropEvent *e)
{
    qDebug() << "Widget::dropEvent";
    QWidget::dropEvent(e);
}
#endif
