#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setText(QString s);
private slots:
    void on_controlBtn_clicked();

    void on_clearBtn_clicked();

private:
    Ui::Widget *ui;

    // 新线程的句柄
    HANDLE hThread;
    DWORD threadId;

    bool isRunning;
};
#endif // WIDGET_H
