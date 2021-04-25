#include "coinbutton.h"

QStringList CoinButton::coinImageList;

CoinButton::CoinButton(QString &fileName, QWidget *parent) :
    QPushButton(parent)
{
    isAnmiation = false;
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    m_coinState = 1;

    setBackgroundImg(fileName);

    connect(this, &CoinButton::clicked, this, &CoinButton::coinBtnClicked);

    // 金币翻银币
    connect(timer1, &QTimer::timeout, [=](){
        setBackgroundImg(coinImageList[flipCount++]);
        if(flipCount >= coinImageList.size()) {
            timer1->stop();
            isAnmiation = false;
        }
    });

    // 银币翻金币
    connect(timer2, &QTimer::timeout, [=](){
        setBackgroundImg(coinImageList[flipCount--]);
        if(flipCount < 0) {
            flipCount = 0;
            timer2->stop();
            isAnmiation = false;
            emit flipComplete(m_row, m_column);
        }
    });
}

// 点击硬币调用的函数
void CoinButton::flipCoin()
{
    isAnmiation = true;
    if(m_coinState) {  // 金币 -> 银币
        m_coinState = 0;
        flipCount = 0;
        timer1->start(30);
    }
    else {  // 银币 -> 金币
        m_coinState = 1;
        flipCount = coinImageList.size()-1;
        timer2->start(30);
    }
}

void CoinButton::setBackgroundImg(QString &imgPath)
{
    QPixmap pm;

    // 设置按钮背景图
    if(pm.load(imgPath)) {
        setFixedSize(pm.width(), pm.height());
        setStyleSheet("QPushButton{border: 0x;}");
        setIcon(pm);
        setIconSize(QSize(pm.width(), pm.height()));
    }
}

void CoinButton::setCoinFlag(int state)
{
    m_coinState = state;
}

void CoinButton::mousePressEvent(QMouseEvent *e)
{
    if(isAnmiation) {
        return;
    }
    else {
        QPushButton::mousePressEvent(e);
    }
}

// 硬币点击事件
void CoinButton::coinBtnClicked()
{
    QSound::play(":/res/ConFlipSound.wav");

    this->flipCoin();

    // 周围延时翻转
    QTimer::singleShot(100, [=]() {
        if(m_row > 0) { // 上方硬币
            (*m_coinBtnPtr)[m_row-1][m_column]->flipCoin();
        }

        if(m_row < (m_coinBtnPtr->size()-1))  { // 下方硬币
            (*m_coinBtnPtr)[m_row+1][m_column]->flipCoin();
        }

        if(m_column > 0) {  // 左侧硬币
            (*m_coinBtnPtr)[m_row][m_column-1]->flipCoin();
        }

        if(m_column < (m_coinBtnPtr[0].size()-1)) {  // 右侧硬币
            (*m_coinBtnPtr)[m_row][m_column+1]->flipCoin();
        }
    });
}

