#ifndef KLISTWIDGET_H
#define KLISTWIDGET_H

#include <QListWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

class KListWidget : public QListWidget
{
public:
    static QStringList walkFileTree(const QString &path);

    KListWidget(QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dragMoveEvent(QDragMoveEvent *e) override;
    void dropEvent(QDropEvent *e) override;
#if 0
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
#endif
public slots:
    void onCustomContextMenuRequested(const QPoint &pos);

    void onDeleteActionTriggered();
    void onClearActionTriggered();

    void onMoveupActionTriggered();
    void onMoveDownActionTriggered();
private:
    int m_preIndex = -1;

    int m_dragRow = -1;
};

#endif // KLISTWIDGET_H
