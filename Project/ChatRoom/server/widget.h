#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <cserver.h>
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

private slots:
    void on_startBtn_clicked();

    void on_closeBtn_clicked();

    void onAcceptEnded();

    void onLogMsgRecv(QString text);
signals:
    void startAcceptThread();
private:
    Ui::Widget *ui;

    CServer *m_Server;
    QThread *m_AcceptThread;
};
#endif // WIDGET_H
