#include "widget.h"
#include "ui_widget.h"
#include <QProgressDialog>
#include <QThread>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_dialogButton_clicked()
{
    QProgressDialog dialog("Downloading...", "Abort", 0, 100, this);

    dialog.setWindowTitle("Download progress");
    dialog.setWindowModality(Qt::WindowModal);    // 模态对话框

    dialog.show();

    for(int i = 0; i <= 100; i++) {
        dialog.setValue(i);
        qApp->processEvents();
        if(dialog.wasCanceled())
            break;
        QThread::msleep(30);
    }
    dialog.setValue(100);
}
