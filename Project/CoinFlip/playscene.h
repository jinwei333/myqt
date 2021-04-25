#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include "mybutton.h"
#include "coinbutton.h"
#include <QLabel>

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    PlayScene(int index, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void backBtnClicked();

public slots:
    void onCoinBtnFliped(int x, int y);
private:

    void coinCheckAround();

    int levelIndex;

    QLabel *winLable;

    QSound *winSound;

    // 硬币状态同步，可用与后期存档
    QVector<QVector<int>> m_LevelData;
    QVector<QVector<CoinButton *>> m_coinBtn;
};

#endif // PLAYSCENE_H
