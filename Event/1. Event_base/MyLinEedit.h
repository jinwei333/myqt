#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QEvent>


class MyLineEdit : public QLineEdit
{
public:
    MyLineEdit(QWidget *parent = nullptr) :
        QLineEdit(parent)
    {

    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MYLINEEDIT_H
