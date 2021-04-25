#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

#if 0
    QPalette paltte;
    paltte.setColor(QPalette::Active, QPalette::WindowText, QColor(Qt::red));
    paltte.setColor(QPalette::Active, QPalette::ButtonText, QColor(Qt::blue));
    paltte.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(Qt::green));
    paltte.setColor(QPalette::Active, QPalette::Highlight, QColor(Qt::black));
    paltte.setColor(QPalette::Active, QPalette::HighlightedText, QColor(Qt::white));
    setPalette(paltte);
#else
    setPalette(QPalette(Qt::red, Qt::green));

#endif

}

Widget::~Widget()
{
    delete ui;
}

