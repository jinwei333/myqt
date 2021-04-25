#ifndef COMBOXDELEGATE_H
#define COMBOXDELEGATE_H
#include <QStyledItemDelegate>
#include <QString>
#include <QStringList>

class ComboxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    static QString currentText;

    ComboxDelegate(const QStringList &l, QObject *parent = Q_NULLPTR)
        : QStyledItemDelegate(parent), names(l) { }

    ~ComboxDelegate() { }
protected:
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

Q_SIGNALS:
    void comboxDelegateEditFinished(int row, int col, const QString&) const;

private:
    const QStringList &names;
};

#endif // COMBOXDELEGATE_H
