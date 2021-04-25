#ifndef COINBUTTON_H
#define COINBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QVector>
#include <QSound>

class CoinButton : public QPushButton
{
    Q_OBJECT
public:
    static QStringList coinImageList;

    CoinButton(QString &fileName, QWidget *parent=nullptr);

    void flipCoin();

    void setCoinFlag(int state);

    void setIndex(int x, int y) {
        m_row = x;
        m_column = y;
    }

    int getCoinState() const { return m_coinState; }

    bool isFliping() const { return isAnmiation; }

    QVector<QVector<CoinButton *>> *m_coinBtnPtr;
Q_SIGNALS:
    void flipComplete(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *e);

public slots:
    void coinBtnClicked();

private:
    void setBackgroundImg(QString &imgPath);

    int m_coinState;  // 1：正面 0：反面
    int m_row;
    int m_column;
    int flipCount = 0;
    QString imagePath;
    QTimer *timer1;
    QTimer *timer2;
    bool isAnmiation = false;   // 是否正在做翻转动作

    QSound *m_clicksound = nullptr;
};

#endif // COINBUTTON_H
