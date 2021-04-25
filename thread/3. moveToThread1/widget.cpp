#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

/**
    如果线程要用到消息循环，使用继承QObject的多线程方法更简单
    继承QObject的多线程不能指定父对象
    把所有耗时操作都作为槽函数
    QMutex会带来一定的耗时，大概速度会降低1.5倍(Release模式)

*/


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_Thread(nullptr)
    , m_Worker(nullptr)
{
    ui->setupUi(this);

    qDebug() << "UI Thread ID: " << QThread::currentThreadId();

    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, 100);

    ui->progressBar2->setValue(0);
    ui->progressBar2->setRange(0, 100);
}

Widget::~Widget()
{
    if(m_Thread) {
        // 退出线程的正确姿势
        m_Thread->quit();
        m_Thread->wait();
    }

    delete ui;
}

void Widget::createThread()
{
    if(m_Thread || m_Worker) {
        qDebug() << "Thread are running!!!";
        return;
    }

    m_Thread = new QThread;
    m_Worker = new Worker;    // 不能指定父对象

    m_Worker->moveToThread(m_Thread);

    // 线程结束后自动销毁
    connect(m_Thread, &QThread::finished, m_Thread, &QThread::deleteLater);

    // 线程结束后销毁 Worker
    connect(m_Thread, &QThread::finished, m_Worker, &Worker::deleteLater);

    // 一个信号同时绑定 m_Worker 的两个槽函数，两个槽函数会依次执行
    connect(this, &Widget::startWork, m_Worker, &Worker::doWork1);
    connect(this, &Widget::startWork, m_Worker, &Worker::doWork2);

    connect(m_Worker, &Worker::setValue1, ui->progressBar, &QProgressBar::setValue);
    connect(m_Worker, &Worker::setValue2, ui->progressBar2, &QProgressBar::setValue);

    connect(m_Thread, &QThread::finished, [](){
        qDebug() << "QThread finished";
    });

    connect(m_Thread, &QThread::destroyed, [=](){
        m_Thread = nullptr;
        m_Worker = nullptr;
        qDebug() << "QThread destoryed";
    });

    m_Thread->start();

    emit startWork();
}


void Widget::on_stopBtn_clicked()
{
#if 0
    if(m_Thread && m_Worker) {
        m_Worker->setStop();
    }
#else
    if(m_Thread) {
        m_Thread->quit();
        m_Thread->wait();
    }

#endif
}

void Widget::on_startBtn_clicked()
{
    createThread();
}
