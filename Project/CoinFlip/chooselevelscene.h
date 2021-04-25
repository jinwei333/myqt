#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include "mybutton.h"
#include "playscene.h"

class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);

protected:

    void paintEvent(QPaintEvent *event) override;
signals:
    void backBtnClicked();
private:
    PlayScene *playScene;
};

#endif // CHOOSELEVELSCENE_H
