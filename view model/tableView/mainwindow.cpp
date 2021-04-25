#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QDebug>
#include <QSpinBox>

#include "spinboxdelegate.h"
#include "comboxdelegate.h"

#define COLUMN  6
#define ROW     10

QStringList items({"组", "音色", "音高", "音量", "声相", "效果器"});
QStringList pads({"TOM1", "TOM1-B", "TOM2", "TOM2-B", "TOM3", "TOM3-B", "SNARE", "SNARE-X-Stick", "SNARE-Rim", "KICK"});

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStandardItemModel(ROW, COLUMN);
    model->setHorizontalHeaderLabels(items);
    model->setVerticalHeaderLabels(pads);
    for(int row = 0; row < ROW; row++) {
        for (int col = 0; col < COLUMN; col++) {
            QStandardItem *item = new QStandardItem(QString::number(row*col));
            model->setItem(row, col, item);
        }
    }

    setComboxDelegate(0, pads);
    setSpinBoxDelegate(1, 0, 100);
    setSpinBoxDelegate(2, 0, 100);
    setSpinBoxDelegate(3, 0, 100);
    setSpinBoxDelegate(4, 0, 100);
    setSpinBoxDelegate(5, 0, 100);

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setFixedHeight(30);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section{border:none;}"
                "QHeaderView::section:checked {border-bottom: 2px solid green;}"
    );

    ui->tableView->verticalHeader()->setStyleSheet(
                "QHeaderView::section{border:none;}"
                "QHeaderView::section:checked {border-right: 2px solid green;}"
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addDelegate()
{
    for(int i = 0; i < ROW; ++i) {
        QComboBox *combox = new QComboBox;

        combox->addItems({"AA", "BB", "CC", "DD"});

        ui->tableView->setIndexWidget(model->index(i, 0), combox);

        connect(combox, &QComboBox::currentTextChanged, [](const QString &s){ qDebug() << s; });

        QSpinBox *spinbox1 = new QSpinBox;
        connect(spinbox1, &QSpinBox::editingFinished, this, [](){ qDebug() << "OK"; });
        ui->tableView->setIndexWidget(model->index(i, 1), spinbox1);
    }
}

void MainWindow::setComboxDelegate(int col, const QStringList &sl)
{
    ComboxDelegate *delegate = new ComboxDelegate(sl, ui->tableView);
    connect(delegate, &ComboxDelegate::comboxDelegateEditFinished, this, &MainWindow::onTabComboxTextChanged);
    ui->tableView->setItemDelegateForColumn(col, delegate);
}

void MainWindow::setSpinBoxDelegate(int col, int min, int max)
{
    SpinBoxDelegate *delegate = new SpinBoxDelegate(min, max, ui->tableView);
    connect(delegate, &SpinBoxDelegate::spinboxDelegateEditFinished, this, &MainWindow::onTabSpinboxEditFinished);
    ui->tableView->setItemDelegateForColumn(col, delegate);
}

void MainWindow::onTabComboxTextChanged(int row, int col, const QString &text)
{
    qDebug() << row << " " << col << " " << text;
}

void MainWindow::onTabSpinboxEditFinished(int row, int col, int value)
{
    qDebug() << row << " " << col << " " << value;
}

