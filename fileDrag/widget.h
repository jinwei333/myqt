#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <KListWidget.h>
#include <QDropEvent>
#include <QDragEnterEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
//    void dragEnterEvent(QDragEnterEvent *e);
//    void dropEvent(QDropEvent *e);
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
