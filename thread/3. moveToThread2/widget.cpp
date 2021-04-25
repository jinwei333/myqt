#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug() << "UI Thread id: " << QThread::currentThreadId();

    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, 100);
    connect(ui->startBtn, &QPushButton::clicked, this, &Widget::createWorkerThread);
}

Widget::~Widget()
{
    if(m_Thread && m_Worker) {
        m_Thread->quit();
        m_Thread->wait();
    }

    delete ui;
}

void Widget::createWorkerThread()
{
    if(m_Thread && m_Worker) {
        QMessageBox::critical(this, "error", "线程已经创建", QMessageBox::Ok);
        return;
    }

    m_Worker = new Worker;
    m_Thread = new MyThread(this);

    m_Worker->moveToThread(m_Thread);

    connect(m_Thread, &MyThread::finished, m_Thread, &MyThread::deleteLater);
    connect(m_Thread, &MyThread::finished, m_Worker, &Worker::deleteLater);
    connect(m_Thread, &MyThread::finished, [](){
        qDebug() << "Thread finished";
    });

    connect(m_Thread, &MyThread::destroyed, [=](){
        m_Thread = nullptr;
        m_Worker = nullptr;
        qDebug() << "Thread destroyed";
    });

    connect(this, &Widget::startWork, m_Worker, &Worker::doWork);
    connect(this, &Widget::stopWork, m_Worker, &Worker::setStop);
    connect(m_Worker, &Worker::setValue1, ui->progressBar, &QProgressBar::setValue);
#if 0
    connect(m_Worker, &Worker::workFinished, [=](){  // QThread::wait: Thread tried to wait on itself
        qDebug() << "workFinished thread id: " << QThread::currentThreadId();
        m_Thread->quit();
        m_Thread->wait();
    });
#else
    connect(m_Worker, &Worker::workFinished, this, &Widget::onWorkFinished);
#endif

    m_Thread->start();

    emit startWork();
}


void Widget::on_stopBtn_clicked()
{
    if(m_Thread && m_Worker) {
        m_Worker->setStop(true);
        m_Thread->quit();
        m_Thread->wait();
        qDebug() << "Thread wait finished";
    }
}

void Widget::onWorkFinished()
{
    m_Thread->quit();
    m_Thread->wait();
}
