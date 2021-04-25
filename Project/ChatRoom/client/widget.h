#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <client.h>
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
    void on_loginBtn_clicked();

    void on_logoutBtn_clicked();

    void on_sendBtn_clicked();

private:
    Ui::Widget *ui;
    Client *m_Client;
};
#endif // WIDGET_H
