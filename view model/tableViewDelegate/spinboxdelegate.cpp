#include "spinboxdelegate.h"
#include <QSpinBox>
#include <QFont>


QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *editor = new QSpinBox(parent);

    QFont font = editor->font();
    font.setBold(true);
    editor->setFont(font);

    editor->setFrame(false);
    editor->setRange(spinbox_min, spinbox_max);

    return  editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox *spinbox = static_cast<QSpinBox *>(editor);
    spinbox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *spinbox = static_cast<QSpinBox *>(editor);

    // We call interpretText() to make sure that we obtain the most up-to-date value in the spin box.
    spinbox->interpretText();

    int value = spinbox->value();
    model->setData(index, value, Qt::EditRole);

    emit spinboxDelegateEditFinished(index.row(), index.column(), value);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    Q_UNUSED(index);
}



