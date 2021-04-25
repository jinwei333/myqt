#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QDebug>
#include <QTimer>

class TableModel : public QAbstractTableModel
{
public:
    TableModel(QObject* parent=nullptr) :
        QAbstractTableModel(parent), timer(new QTimer(this)) {

        timer->setInterval(1000);
        connect(timer, &QTimer::timeout, this, &TableModel::timerHint);
        timer->start();
    }
public slots:
    void timerHint();
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QTimer *timer;

    QString m_Data[3][4];
};

#endif // TABLEMODEL_H
