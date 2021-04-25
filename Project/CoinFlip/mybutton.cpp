#include "mybutton.h"
#include <QMessageBox>
#include <QPropertyAnimation>   // 动画

MyButton::MyButton(QString normalImgPath, QString pressImgPath)
{
    normalImagePath = normalImgPath;
    pressImagePath = pressImgPath;

    setButtonImage(normalImgPath);
}

// 向下跳跃
void MyButton::zoomDown()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

    // 设置动画间隔
    animation->setDuration(100);

    // 设置起始位置
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 设置结束位置
    animation->setEndValue(QRect(this->x(), this->y()+20, this->width(), this->height()));

    // 设置跳跃曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

// 向上跳跃
void MyButton::zoomUp()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");

    // 设置动画间隔
    animation->setDuration(100);

    // 设置起始位置
    animation->setStartValue(QRect(this->x(), this->y()+20, this->width(), this->height()));

    // 设置结束位置
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 设置跳跃曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    // 停止后删除对象
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MyButton::mousePressEvent(QMouseEvent *e)
{
    setButtonImage(pressImagePath);

    QPushButton::mousePressEvent(e);
}

void MyButton::mouseReleaseEvent(QMouseEvent *e)
{
    setButtonImage(normalImagePath);

    QPushButton::mouseReleaseEvent(e);
}

void MyButton::setButtonImage(QString &imgPath)
{
    QPixmap pm;
    if(pm.load(imgPath) != true) {
        return;
    }
    // 设置按钮固定尺寸
    setFixedSize(pm.width(), pm.height());
    // 设置不规则样式
    setStyleSheet("QPushButton {border: 0;}");
    // 设置按钮图标
    setIcon(pm);
    setIconSize(QSize(pm.width(), pm.height()));
}
