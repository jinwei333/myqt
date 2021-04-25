#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QTableView>
#include <QGridLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QDebug>
#include <QSpinBox>
#include "spinboxdelegate.h"
#include "comboxdelegate.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addDelegate();

    void comboxIndexChanged(const QString &s);

    void setComboxDelegate(int col, const QStringList &sl);
    void setSpinBoxDelegate(int col, int min, int max);

public Q_SLOT:
    void onTabComboxTextChanged(int, int, const QString &);
    void onTabSpinboxEditFinished(int, int, int);
Q_SIGNALS:

private:
    Ui::MainWindow *ui;

    QStandardItemModel *model;
};
#endif // MAINWINDOW_H
