#include "widget.h"
#include "ui_widget.h"
#include <QRunnable>
#include <QThreadPool>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 最大线程数量（4）
    qDebug() << QThread::idealThreadCount();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_startBtn_clicked()
{
    MyRunnable *runnable1 = new MyRunnable("Thread1");
    runnable1->setAutoDelete(true);

    MyRunnable *runnable2 = new MyRunnable("Thread2");
    runnable2->setAutoDelete(true);

    MyRunnable *runnable3 = new MyRunnable("Thread3");
    runnable2->setAutoDelete(true);

    MyRunnable *runnable4 = new MyRunnable("Thread4");
    runnable2->setAutoDelete(true);

    MyRunnable *runnable5 = new MyRunnable("Thread5");
    runnable2->setAutoDelete(true);

    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->start(runnable1);
    threadPool->start(runnable2);
    threadPool->start(runnable3);
    threadPool->start(runnable4);
    threadPool->start(runnable5);
}
