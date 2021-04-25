#include "widget.h"
#include "ui_widget.h"
#include <QList>
#include <QFileSystemModel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

#if 1
    // 创建模型
    m_treeModel = new QStandardItemModel(ui->treeView);

//    m_treeModel->setHorizontalHeaderLabels({"One", "Two", "Three"});

#if 1
    QStandardItem *rootNode = m_treeModel->invisibleRootItem();

    QList<QStandardItem *> headerRow;
    headerRow.append(new QStandardItem("Name"));
    headerRow.append(new QStandardItem("class"));
    headerRow.append(new QStandardItem("score"));

    rootNode->appendRow(headerRow);
#endif

    QList<QStandardItem*> secondRow;
    secondRow.append(new QStandardItem("Jimi"));
    secondRow.append(new QStandardItem("一"));
    secondRow.append(new QStandardItem("100"));

    secondRow.clear();
    secondRow.append(new QStandardItem("Lami"));
    secondRow.append(new QStandardItem("二"));
    secondRow.append(new QStandardItem("90"));

    headerRow.first()

    QList<QStandardItem*> thirdRow;
    thirdRow.append(new QStandardItem("100"));
    thirdRow.append(new QStandardItem("80"));
    thirdRow.append(new QStandardItem("68"));
    secondRow.first()->appendRow(thirdRow);

    ui->treeView->header()->hide();
    ui->treeView->setModel(m_treeModel);
    ui->treeView->expandAll();
#else
    QFileSystemModel *model = new QFileSystemModel(this);
    model->setRootPath(QDir::currentPath());
    model->setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    ui->treeView->setModel(model);
#endif
}

Widget::~Widget()
{
    delete ui;
}

