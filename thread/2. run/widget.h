#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mythread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    MyThread *thread2Create();

    void setBtnText();
signals:
    void stopThread();

private slots:
    void on_controlBtn_clicked();

    void on_controlBtn2_clicked();

    void onThreadDestoryed(QObject *obj);

protected:
    void closeEvent(QCloseEvent *e);

private:
    Ui::Widget *ui;

    MyThread *m_Thread1 = nullptr;    // 全局线程
    MyThread *m_Thread2 = nullptr;    // 局部线程

};
#endif // WIDGET_H
