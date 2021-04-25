#ifndef SPINBOXDELEGATE_H
#define SPINBOXDELEGATE_H
#include <QStyledItemDelegate>

class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SpinBoxDelegate(int min, int max, QObject *parent = Q_NULLPTR)
        : QStyledItemDelegate(parent), spinbox_min(min), spinbox_max(max){ }

protected:
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

Q_SIGNALS:
    void spinboxDelegateEditFinished(int row, int col, int index) const;

private:
    int spinbox_min;
    int spinbox_max;

};

#endif // SPINBOXDELEGATE_H
