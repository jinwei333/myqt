比较好的博文

[Qt线程与事件循环的正确用法](https://www.cnblogs.com/icmzn/p/7348264.html)

> *The run() implementation is for a thread what the main() entry point is for the application. All code executed in a call stack that starts in the run() function is executed by the new thread, and the thread finishes when the function returns.*
>
> run 对于线程的作用相当于main函数对于应用程序。它是线程的入口，run的开始和结束意味着线程的开始和结束。

QThread 是用来**管理**线程的，它所 **依附的线程** 和 它 **管理的线程**不是一个概念

- 依附的线程：创建 QThread 对象的线程（通常是主线程）
- 管理的线程：run 函数。

因为 QThread 对象依附在主线程中，所以 slot 函数会在主线程中执行。

如果改变 QThread 对象的依附性（moveToThread），且信号槽连接方式为 Qt::DirectConnection，此时 slot 函数将会在子线程中执行。



**可重入的**（Reentrant）：如果多个线程可以在同一时刻调用一个类的所有函数，并且保证每一次函数调用都引用一个唯一的数据，就称这个类是可重入的（Reentrant means that all the functions in the referenced class can be called simultaneously by multiple threads, provided that each invocation of the functions reference unique data.）。

**线程安全**（Thread-safe）：如果多个线程可以在同一时刻调用一个类的所有函数，即使每一次函数调用都引用一个共享的数据，就说这个类是线程安全的（Threadsafe means that all the functions in the referenced class can be called simultaneously by multiple threads even when each invocation references shared data.）。如果多个线程可以在同一时刻访问函数的共享数据，就称这个函数是线程安全的。

QThread 中的 run 默认实现只是简单的调用了 **QThread::exec** 函数。

QThreadPool::globalInstance()  全局线程池



## QObject::connect

```c++
[static] QMetaObject::Connection QObject::connect(
    const QObject *sender,
    const char *signal,
    const QObject *receiver,
    const char *method,
    Qt::ConnectionType type = Qt::AutoConnection)
```



**Qt::AutoConnection**

- 这是默认设置
- 如果信号在接收者所依附的线程内发射，则等同于直接连接
- 如果发射信号的线程和接受者所**依附的线程**不同，则等同于队列连接

**Qt::DirectConnection**

- 当信号发射时，槽函数将直接被调用。
- 无论槽函数所属对象在哪个线程，**槽函数都在发射信号的线程内执行**。

**Qt::QueuedConnection**

- 当控制权回到接受者所依附线程的事件循环时，槽函数被调用。
- 槽函数在接收者所依附线程执行。





## moveToThread

> Changes the thread affinity for this object and its children. The object cannot be moved if it has a parent. Event processing will continue in the *targetThread*.

线程运行一次就销毁

**`QThread::quit()`和`QThread::exit()`函数来终止事件循环**。

```c++
/** 子类化QThread只用于调试观察 */
class MyThread :public QThread
{
public:
    MyThread(QObject *parent = nullptr) : QThread(parent) {}
    ~MyThread() { qDebug() << "~MyThread"; }
    void run() {
        int ret = 0;
        qDebug() << "Enter run function" << QThread::currentThreadId();
        ret = QThread::exec();	// QThread::run 的默认实现
        qDebug() << "Leave run function, ret = " << ret;
    }
};

```



```c++
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    ~Worker();
public slots:
    void doWork() {
        qDebug() << "Worker thread id: " << QThread::currentThreadId();
        for(int i = 0; i < 100; i++) {
            emit setValue(i + 1);
            QThread::msleep(20);
            if(isStop())
                break;
        }
        emit workFinished();
    }
    bool isStop() const { return m_Stop; }
    void setStop(bool flag) { m_Stop = flag; }
signals:
    void setValue(int);
    void workFinished();
private:
    bool m_Stop;
};
```



```c++

Widget::~Widget() {
    if(m_Thread && m_Worker) {	// 析构函数先退出子线程
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

    connect(m_Thread, &MyThread::finished, m_Thread, &MyThread::deleteLater);//线程结束后销毁m_Thread
    connect(m_Thread, &MyThread::finished, m_Worker, &Worker::deleteLater);	//线程结束后销毁 Worker
    connect(m_Thread, &MyThread::finished, [](){ qDebug() << "Thread finished"; });
    connect(m_Thread, &MyThread::destroyed, [=](){
        m_Thread = nullptr;
        m_Worker = nullptr;
        qDebug() << "Thread destroyed";
    });
    connect(this, &Widget::startWork, m_Worker, &Worker::doWork);	// 启动线程
    connect(m_Worker, &Worker::setValue, ui->progressBar, &QProgressBar::setValue);
 	connect(m_Worker, &Worker::workFinished, this, &Widget::onWorkFinished);

    m_Thread->start();
    emit startWork();	// 启动槽函数
}

void Widget::onWorkFinished() {
    m_Thread->quit();
    m_Thread->wait();
}

void Widget::on_stopBtn_clicked() {
    if(m_Thread && m_Worker) {
        m_Worker->setStop(true);
        m_Thread->quit();
        m_Thread->wait();
        qDebug() << "Thread wait finished";
    }
}
```

`UI Thread id:  0x1c20``
``Enter run function 0xa28``
``Worker1 thread id:  0xa28``
``Leave run function, ret =  0``
``Thread finished``
``~Worker``
``~MyThread``
``Thread destroyed`



