#include "KListWidget.h"
#include <QMimeData>
#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QDrag>

/***
 * 功能：1. 拖拽文件追加到 listWidget后。如果是目录，遍历目录
 *      2. 列表右键菜单：删除选中项，清除列表，上移，下移
 *      3. 文件鼠标拖动列表项
 *
 **/


QStringList KListWidget::walkFileTree(const QString &path)
{
    QStringList fileList;
    QDir dir(path);

    // QList<QFileInfo>
    QFileInfoList infoList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < infoList.count(); i++) {
        const QFileInfo& info = infoList.at(i);
        QString fileName = info.absoluteFilePath();

        if(info.isDir()) {
            QStringList subfile = walkFileTree(fileName);   // 递归
            fileList.append(subfile);
        }
        else {
            fileList.append(fileName);
        }
    }
    return fileList;
}

KListWidget::KListWidget(QWidget* parent) :
    QListWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
//    setDragDropMode(QAbstractItemView::DragDrop);
//    setDefaultDropAction(Qt::CopyAction);
//    setDropIndicatorShown(true);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &KListWidget::customContextMenuRequested,
            this, &KListWidget::onCustomContextMenuRequested);

    setSelectionMode(QAbstractItemView::ContiguousSelection);

    setFocusPolicy(Qt::NoFocus);
}

void KListWidget::dragEnterEvent(QDragEnterEvent *e)
{
    KListWidget *src = (KListWidget *)e->source();

    if(src && src==this) {
        m_dragRow = currentRow();
        e->setDropAction(Qt::CopyAction);
        e->accept();
    }
    else {
        if(e->mimeData()->hasUrls()) {
            e->acceptProposedAction();
            QListWidget::dragEnterEvent(e);

            // The same as accept(), but also notifies that future moves will also be acceptable
            // if they remain within the rectangle given on the widget.This can improve performance,
            // but may also be ignored by the underlying system.
            e->accept();
        }
        else {
            qDebug() << "ignore";
            e->ignore();    // 传递给父组件(QListWidget) 处理
        }

        qDebug() << "外部::dragEnterEvent";
    }
}

void KListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    KListWidget *src = (KListWidget *)e->source();

    if(src && src==this) {
        e->setDropAction(Qt::CopyAction);
        e->accept();
//        qDebug() << "内部::dragMoveEvent";
    }
    else {
        QListWidget::dragMoveEvent(e);
        e->accept();
    }
}

void KListWidget::dropEvent(QDropEvent *e)
{
    KListWidget *src = (KListWidget *)e->source();

    if(m_dragRow!=-1 && src && src==this) {
        int newRow = row(itemAt(e->pos()));
        if(m_dragRow!=newRow && newRow!=-1)
        {
            QString dragText = item(m_dragRow)->text();
            item(m_dragRow)->setText(item(newRow)->text());
            item(newRow)->setText(dragText);
        }
        m_dragRow = -1;
        e->setDropAction(Qt::CopyAction);
        e->accept();
    }
    else {
        const QMimeData* mimeData = e->mimeData();

        if(!mimeData->hasUrls())
            return;
        QList<QUrl> urls = mimeData->urls();
        QStringList fileList;

        for(int i = 0; i < urls.count(); i++) {
            const QUrl& url = urls.at(i);

            QString fileName = url.toLocalFile();
            if(QFileInfo(fileName).isDir()) // 如果是目录， 遍历后添加到 fileList
                fileList.append(walkFileTree(fileName));
            else
                fileList.append(fileName);
        }
        if(!fileList.isEmpty())
            addItems(fileList);

        qDebug() << "外部::dropEvent";
    }
}
#if 0
void KListWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        QPoint pos = e->pos();
        QListWidgetItem *item = itemAt(pos);

        qDebug() << "row: " << row(item);
        if(item == NULL) {
            setCurrentRow(-1);
        }
        else {
            qDebug() << "mouse pressed";
            m_dragItem = item;
        }
    }
    else {
        QPoint pos = e->pos();

        QListWidgetItem *item = itemAt(pos);
        qDebug() << item;
        if(item == NULL) {
            setCurrentRow(-1);
        }
    }
    QListWidget::mousePressEvent(e);
}

void KListWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(m_dragItem && (e->button()==Qt::LeftButton)) {
        qDebug() << "mouse move";

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(m_dragItem->text());
        drag->setMimeData(mimeData);

        if(drag->exec(Qt::CopyAction) == Qt::CopyAction) {

        }
        delete drag;
    }

    QListWidget::mouseMoveEvent(e);
}

void KListWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton) {
        if(m_dragItem) {
            qDebug() << "mouse released";
            m_dragItem = NULL;
        }
    }
    QListWidget::mouseReleaseEvent(e);
}
#endif

void KListWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = itemAt(pos);

    if(item == nullptr)
        return;

    qDebug() << "onCustomContextMenuRequested" << row(item);

    QMenu *popMenu = new QMenu(this);
    QAction *deleteAct = new QAction(tr("delete"), this);
    QAction *clearAct = new QAction(tr("clear"), this);
    QAction *moveup = new QAction(tr("move up"), this);
    QAction *movedown = new QAction(tr("move down"), this);
    popMenu->addAction(deleteAct);
    popMenu->addAction(clearAct);
    connect(deleteAct, &QAction::triggered, this, &KListWidget::onDeleteActionTriggered);
    connect(clearAct, &QAction::triggered, this, &KListWidget::onClearActionTriggered);

    popMenu->exec(QCursor::pos());

    delete popMenu;
    delete clearAct;
    delete deleteAct;
    delete moveup;
    delete movedown;
}

void KListWidget::onDeleteActionTriggered()
{
    QList<QListWidgetItem *> selected = selectedItems();

    if(!selected.count())
        return;
    for(int i = 0; i < selected.count(); i++) {
        QListWidgetItem *item = selected.at(i);

        if(item != NULL) {
            takeItem(row(item));
            delete item;
        }
    }
}

void KListWidget::onClearActionTriggered()
{
    clear();
}

void KListWidget::onMoveupActionTriggered()
{

}

void KListWidget::onMoveDownActionTriggered()
{

}



