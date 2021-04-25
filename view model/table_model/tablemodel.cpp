#include "tablemodel.h"

#include <QFont>
#include <QBrush>
#include <QTime>

void TableModel::timerHint()
{
    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(2, 0);

    emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return  3;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}


QVariant TableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();


    switch (role) {
    case Qt::DisplayRole:
        // 第一列显示时间
        if(col==0) {
            return QTime::currentTime().toString();
        }
        else {
            return m_Data[row][col];
//            return QString("Row%1, Column%2").arg(row).arg(col);
        }
        break;
    case Qt::EditRole:
        return m_Data[row][col];
    case Qt::FontRole:
        if(col == 0) {
            QFont font;
            font.setBold(true);
            return font;
        }
        break;
    case Qt::BackgroundRole:
        if(col == 0) {
            return QBrush(Qt::green);
        }
        break;
    case Qt::CheckStateRole:
        if(col == columnCount()-1) {
            return  Qt::Checked;
        }
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // 设置行标题
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QString("one");
        case 1:
            return QString("two");
        case 2:
            return QString("three");
        case 3:
            return QString("four");
        }
    }
    // 设置行标题的字体
    if(role==Qt::FontRole  && orientation==Qt::Horizontal)
    {
        return QFont("Times", 14, QFont::Bold);
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        if(!index.isValid())
            return false;

        m_Data[index.row()][index.column()] = value.toString();

        qDebug() << "Edit finished: " << value.toString();

        return true;
    }

    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(index.column() != 0) {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }

    return QAbstractTableModel::flags(index);
}
