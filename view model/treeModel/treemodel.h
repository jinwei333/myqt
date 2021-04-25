#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include <QStandardItem>

class TreeModel : public QStandardItemModel
{
public:
    TreeModel(QObject *parent = Q_NULLPTR) :
        QStandardItemModel(parent)
    {

    }

protected:


};

#endif // TREEMODEL_H
