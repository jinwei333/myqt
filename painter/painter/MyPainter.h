#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class MyPainter : public QWidget
{
    Q_OBJECT
public:
    explicit MyPainter(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
signals:

};

#endif // MYPAINTER_H
