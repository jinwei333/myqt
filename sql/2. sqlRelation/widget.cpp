#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->salarySpinBox->setRange(2000, 100000);

    openDataBase();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openDataBase()
{
    QSqlDatabase employeeDb = QSqlDatabase::addDatabase("QSQLITE");

    employeeDb.setDatabaseName(":memory:");
    if(employeeDb.open() != true) {
        QMessageBox::critical(this, tr("Can not open database"),
                              tr("Unable to establish a database connection.\n"
                                 "This example needs SQLite support. Please read "
                                 "the Qt SQL driver documentation for information how "
                                 "to build it."), QMessageBox::Cancel);
        return;
    }
    QSqlQuery query;

    query.exec("create table employee(id int primary key, name varchar(32), salary int, departid int)");

    query.exec("insert into employee values(1, 'Alice', 10000, 100)");
    query.exec("insert into employee values(2, 'Jane', 12000, 100)");
    query.exec("insert into employee values(3, 'Blob', 30000, 101)");
    query.exec("insert into employee values(4, 'Sam', 60000, 102)");
    query.exec("insert into employee values(5, 'Lin', 100000, 103)");

    // 创建部门映射表
    query.exec("create table department(departType int, description varchar(20))");
    query.exec("insert into department values(100, 'worker')");
    query.exec("insert into department values(101, 'manager')");
    query.exec("insert into department values(102, 'president')");
    query.exec("insert into department values(103, 'boss')");

    if(!query.exec("select id,name,salary,departid from employee")) {
        QMessageBox::critical(this, tr("Database error"), employeeDb.lastError().text());
        return;
    }

    while (query.next()) {
        qDebug() << query.value(0).toInt() << ' ' << query.value(1).toString() << ' '
                 << query.value(2).toDouble() << ' ' << query.value(3).toString();
    }

    m_Model = new QSqlRelationalTableModel(this);
    m_Model->setTable("employee");
    m_Model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    int departIndex = m_Model->fieldIndex("departid");
    m_Model->setRelation(departIndex, QSqlRelation("department", "departType", "description"));
    m_Model->select();

    QSqlTableModel *relationModel = m_Model->relationModel(departIndex);
    ui->departComboBox->setModel(relationModel);
    ui->departComboBox->setModelColumn(relationModel->fieldIndex("description"));

    m_Mapper = new QDataWidgetMapper(this);
    m_Mapper->setModel(m_Model);
    m_Mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    m_Mapper->addMapping(ui->idSpinBox, m_Model->fieldIndex("id"));
    m_Mapper->addMapping(ui->nameEdit, m_Model->fieldIndex("name"));
    m_Mapper->addMapping(ui->salarySpinBox, m_Model->fieldIndex("salary"));
    m_Mapper->addMapping(ui->departComboBox, departIndex);

    connect(ui->prevBtn, &QPushButton::clicked, m_Mapper, &QDataWidgetMapper::toPrevious);
    connect(ui->nextBtn, &QPushButton::clicked, m_Mapper, &QDataWidgetMapper::toNext);
    connect(m_Mapper, &QDataWidgetMapper::currentIndexChanged, this, &Widget::updateButton);

    m_Mapper->toFirst();
}

void Widget::updateButton(int row)
{
    ui->prevBtn->setEnabled(row > 0);
    ui->nextBtn->setEnabled(row < (m_Model->rowCount()-1));
}

