#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    static QEvent::Type myType;
public:

    Widget(QWidget *parent = nullptr);
    ~Widget();

    void sendKeyEvent(int key);

    void customEvent(QEvent *ev);

    void myDelay(int msec);

private slots:
    void on_backBtn_clicked();

    void on_deleteBtn_clicked();

    void on_enterBtn_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
