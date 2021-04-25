#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

/**
   全局线程：线程在创建时会把生成线程的窗体作为它的父对象，这样窗体结束时会自动析构线程的对象。
            关闭窗口，子线程未结束，会导致程序崩溃。

   局部线程：某时刻才会触发，临时完成一个耗时操作。
            例如：子线程中显示一个大图片，还没显示完就切换下一张图片，此时应该怎么操作。
                 一般操作是销毁当前线程，创建一个新线程显示下一张图片

            创建方式：1 创建时不要指定父对象，
                    2 connect(thread, &MyThread::finished, thread, &MyThread::deleteLater);

*/

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug() << "UI thread id: " << QThread::currentThreadId();

    // 全局线程
    m_Thread1 = new MyThread(this);
    m_Thread1->setCount(0);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(m_Thread1->getCount());

    connect(m_Thread1, &MyThread::sendRunCount,  ui->progressBar, &QProgressBar::setValue);
    connect(this, &Widget::stopThread, m_Thread1, &MyThread::onThreadStop);

    connect(m_Thread1, &MyThread::finished, [](){
        qDebug() << "Thread1 is finished";
    });
    setBtnText();

    // 局部线程
    m_Thread2 = thread2Create();
    m_Thread2->start();

    thread();
}

Widget::~Widget()
{
    // 让子线程结束运行
    m_Thread1->onThreadStop();

    // 主线程等待子线程结束
    m_Thread1->wait();

    qDebug() << "~Widget";
    delete ui;
}


// finished -> deleteLater --> ~MyThread --> destroyed
MyThread *Widget::thread2Create()
{
    // 局部线程 不需要指定父对象
    MyThread* thread = new MyThread();
    ui->progressBar2->setRange(0, 100);
    ui->progressBar2->setValue(0);
    connect(thread, &MyThread::sendRunCount, ui->progressBar2, &QProgressBar::setValue);

    // 令线程结束时销毁自己，
    // 不再需要在析构函数中等待子线程结束
    connect(thread, &MyThread::finished, thread, &QObject::deleteLater);

    // Thread2 销毁后调用槽函数
    connect(thread, &MyThread::destroyed, this, &Widget::onThreadDestoryed);

    connect(thread, &MyThread::finished, [](){
        qDebug() << "Thread2 is finished";
    });

    return thread;
}

void Widget::setBtnText()
{
    if(m_Thread1->isRunning()) {
        ui->controlBtn->setText("Stop");
    }
    else {
        ui->controlBtn->setText("Start");
    }
}


void Widget::on_controlBtn_clicked()
{
    qDebug() << m_Thread1;

    if(m_Thread1->isRunning()) {
        emit stopThread();
    }
    else {
        m_Thread1->start();
    }
    setBtnText();
}

void Widget::on_controlBtn2_clicked()
{
    if(m_Thread2) {
        m_Thread2->onThreadStop();
    }
    m_Thread2 = thread2Create();
    m_Thread2->start();
}

// 本地资源回收
void Widget::onThreadDestoryed(QObject *obj)
{
    qDebug() << "Thread2 destoryed";

    if(m_Thread2 == obj) {
        m_Thread2 = nullptr;
    }
}

void Widget::closeEvent(QCloseEvent *e)
{
    if(m_Thread2) {
        if(m_Thread2->isRunning()) {
           m_Thread2->onThreadStop();
           m_Thread2->wait();
        }
    }
    qDebug() << "CloseEvent";
}
