#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setTableModel();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setTableModel()
{
    m_tableModel = new TableModel(this);
    m_treeModel  = new TableModel(this);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(m_tableModel);
}

