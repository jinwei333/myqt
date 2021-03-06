#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include <QPainter>
#include "chooselevelscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainScene; }
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();
protected:
    void paintEvent(QPaintEvent *event) override;
private:

    ChooseLevelScene *levelScene;

    Ui::MainScene *ui;
};
#endif // MAINSCENE_H
