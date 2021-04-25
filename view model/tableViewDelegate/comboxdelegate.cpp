#include "comboxdelegate.h"
#include <QComboBox>

QString ComboxDelegate::currentText;

QWidget *ComboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QString text = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *combox = new QComboBox(parent);

    combox->addItems(names);
    int i = names.indexOf(text);
    combox->setCurrentIndex(i);
    combox->setMaxVisibleItems(10);
    combox->setFrame(false);
    combox->showPopup();

    return combox;
}

void ComboxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
}

void ComboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combox = static_cast<QComboBox*>(editor);

    currentText = combox->currentText();
    model->setData(index, currentText, Qt::EditRole);

    emit comboxDelegateEditFinished(index.row(), index.column(), currentText);
}

void ComboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}


