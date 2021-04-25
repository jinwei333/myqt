#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QThread>

// typedef DWORD (WINAPI *PTHREAD_START_ROUTINE) (LPVOID lpThreadParameter);
DWORD WINAPI threadFunc (LPVOID lpThreadParameter)
{
    int count = 0;
    Widget *widget = (Widget *)lpThreadParameter;
    while(TRUE) {
        widget->setText(QString::number(count++));
        Sleep(200);
    }
    qDebug() << "线程结束";
    return 0;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    isRunning = false;

    hThread = CreateThread(
                NULL,  // 线程内核对象的安全属性，一般传入NULL表示使用默认设置。
                0,     // 线程栈空间大小。传入0 表示使用默认大小（1MB）。
                threadFunc,  // 新线程所执行的线程函数地址
                this,  // 传给线程函数的参数
                CREATE_SUSPENDED, // 额外的标志来控制线程的创建，为0表示线程创建之后立即就可以进行调度，如果为
                       // CREATE_SUSPENDED 则表示线程创建后暂停运行，这样它就无法调度，直到调用 ResumeThread()。

                &threadId  // 返回线程的ID号，传入NULL表示不需要返回该线程ID号。
                );

    qDebug() << "UI 线程的线程 ID：" << QThread::currentThreadId();
    qDebug() << "新线程的线程 ID：" << threadId;


}

Widget::~Widget()
{
    // 此函数调用后终止的线程堆栈资源不会得到释放。一般不建议使用此函数。
    TerminateThread(hThread, 0);

    // 等待函数 –使线程进入等待状态，直到指定的内核对象被触发。
    WaitForSingleObject(hThread, 2000);

    delete ui;
}

void Widget::setText(QString s)
{
    ui->textEdit->append(s);
}


void Widget::on_controlBtn_clicked()
{
    if(isRunning == false) {
        if(ResumeThread(hThread)!=-1U) {
            isRunning = true;
            ui->controlBtn->setText("挂起线程");
        }
    }
    else {

        if(SuspendThread(hThread) != -1U) {
            isRunning = false;
            ui->controlBtn->setText("启动线程");
        }
    }
}

void Widget::on_clearBtn_clicked()
{
    ui->textEdit->clear();
}
