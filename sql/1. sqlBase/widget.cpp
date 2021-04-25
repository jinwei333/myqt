#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlQueryModel>
#include <QTableView>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->tableView->verticalHeader()->setHidden(true);
    qDebug() << QSqlDatabase::drivers();    // ("QSQLITE", "QMYSQL", "QMYSQL3", "QODBC", "QODBC3", "QPSQL", "QPSQL7")
}

Widget::~Widget()
{
    if(m_db.isOpen()) {
        m_db.close();
    }

    delete ui;
}

bool Widget::sqlQuery(const QString sql)
{
    QSqlQuery query;
    if(!query.exec(sql)) {
        QMessageBox::critical(this, tr("sqlQuery error"), m_db.lastError().text());
        return false;
    }
    return true;
}

void Widget::on_openBtn_clicked()
{
    if(m_db.isOpen() == true) {
        QMessageBox::critical(this, tr("Database open"), "The database connection is already open");
        return;
    }

    // 创建数据模型
    m_Model = new QSqlQueryModel(this);
    m_Model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_Model->setHeaderData(1, Qt::Horizontal, tr("NAME"));

    m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName("mydb.db");

    if(m_db.open() != true) {
        QMessageBox::critical(this, tr("Database error"), m_db.lastError().text());
        return;
    }
    on_queryBtn_clicked();
    QMessageBox::information(this, tr("Database"), "Database open suceess!");
}

void Widget::on_queryBtn_clicked()
{
#if 0
    if(!m_db.isOpen()) {
        QMessageBox::critical(this, tr("Database open"), "The database connection is not open");
        return;
    }
    QSqlQuery query;
//    query.exec("create table student(id int primary key,name varchar)");
//    query.exec("insert into student values(1,'xiaogang')");
//    query.exec("insert into student values(2,'xiaoming')");
//    query.exec("insert into student values(3,'xiaohong')");

    if(!query.exec("select id,name from student")) {
        QMessageBox::critical(this, tr("Database error"), m_db.lastError().text());
        return;
    }

    ui->textEdit->clear();
    while (query.next()) {
        ui->textEdit->append(query.value(0).toString() + ' ' + query.value(1).toString());
    }
#endif

    if(!m_Model)
        return;

    m_Model->setQuery("select id,name from student");
    ui->tableView->setModel(m_Model);
}

void Widget::on_insertBtn_clicked()
{
    QSqlQuery query;
    QString id = ui->idSpinBox->text();
    QString name(ui->nameLineEdit->text());

    if(true != query.exec(QString("insert into student values(%1,'%2')").arg(id).arg(name))) {
        QMessageBox::critical(this, tr("Insert"), m_db.lastError().text());
        return;
    }
    else {
        on_queryBtn_clicked();
        QMessageBox::information(this, tr("Insert"), QString("Insert %1,'%2' OK!").arg(id).arg(name));
    }
}

void Widget::on_deleteBtn_clicked()
{
    QSqlQuery query;

    if(true != query.exec(QString("delete from student where id==%1 ").arg(ui->delSpinBox->value()))) {
        QMessageBox::critical(this, tr("Delete"), m_db.lastError().text());
        return;
    }
    else {
        on_queryBtn_clicked();
        QMessageBox::information(this, tr("Delete"), "delete success!");
    }
}

void Widget::on_clearBtn_clicked()
{
    ui->tableView->setModel(nullptr);
}
