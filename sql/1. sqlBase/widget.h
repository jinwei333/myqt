#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool sqlQuery(const QString sql);

private slots:
    void on_openBtn_clicked();

    void on_queryBtn_clicked();

    void on_insertBtn_clicked();

    void on_deleteBtn_clicked();

    void on_clearBtn_clicked();

private:
    Ui::Widget *ui;

    QSqlDatabase m_db;

    QSqlQueryModel *m_Model = nullptr;
};
#endif // WIDGET_H
