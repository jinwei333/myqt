#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "worker.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void createThread();

signals:
    void startWork();

private slots:
    void on_stopBtn_clicked();

    void on_startBtn_clicked();

private:
    Ui::Widget *ui;

    Worker *m_Worker = nullptr;

    QThread *m_Thread = nullptr;
};
#endif // WIDGET_H
