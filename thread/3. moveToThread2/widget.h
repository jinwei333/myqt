#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myThread.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void createWorkerThread();
private slots:
    void on_stopBtn_clicked();

    void onWorkFinished();

signals:
    void startWork();
    void stopWork(bool);
private:
    Ui::Widget *ui;

    Worker *m_Worker = nullptr;
    MyThread *m_Thread = nullptr;
};
#endif // WIDGET_H
