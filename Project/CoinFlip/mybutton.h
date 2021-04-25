#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    // normalImg: 正常显示图片
    // pressImg: 按下去显示图片
    MyButton(QString normalImg, QString pressImg = "");

    void zoomDown();
    void zoomUp();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
signals:

private:
    void setButtonImage(QString &imgPath);

    QString normalImagePath;
    QString pressImagePath;
};

#endif // MYBUTTON_H
