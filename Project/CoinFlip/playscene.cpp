#include "playscene.h"
#include <QTimer>
#include <QLabel>
#include "coinbutton.h"
#include "dataconfig.h"
#include <QDebug>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QSound>

PlayScene::PlayScene(int index, QWidget *parent):
    QMainWindow(parent), levelIndex(index)
{
    winSound = new QSound(":/res/LevelWinSound.wav", this);

    // 设置窗口尺寸
    this->resize(320, 588);
    // 设置窗口标题
    this->setWindowTitle("CoinFlip");
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    // 创建返回按钮
    MyButton *backBtn = new MyButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(), this->height()-backBtn->height());

    connect(backBtn, &MyButton::clicked, [=](){
        QSound::play(":/res/BackButtonSound.wav");
        // 延时100ms，发送返回按钮点击信号
        QTimer::singleShot(100, [=](){ emit backBtnClicked(); });

    });

    // Label 显示当前关卡号
    QLabel *label = new QLabel(this);
    label->setGeometry(0, 50, this->width()-10, 30);
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    label->setStyleSheet("color: #DCDCDC;");
    label->setFont(QFont("微软雅黑", 18, QFont::Bold));
    label->setText(QString("Level: %1").arg(levelIndex));

    dataConfig config;

    const int rowNum = config.mData[index].size();
    const int colNum = config.mData[index][0].size();

    QPixmap pm;

    // 胜利图片显示
    winLable = new QLabel(this);
    pm.load(":/res/LevelCompletedDialogBg.png");
    winLable->setPixmap(pm);
    winLable->setGeometry(this->width()*0.5-pm.width()*0.5, -pm.height(), pm.width(), pm.height());

    // 绘制金币
    if(pm.load(":/res/BoardNode.png")) {
        for(int i = 0; i < rowNum; i++) {
            QVector<CoinButton *> btns;
            for(int j = 0; j < colNum; j++) {
                QLabel *label = new QLabel(this);

                label->setPixmap(pm);

                label->setGeometry((this->width() - pm.width()*4)*0.5+i*50,
                                   (this->height() - pm.height()*4)*0.5+j*50,
                                   pm.width(), pm.height());


                int coinState = config.mData[index][i][j];

                QString str;
                if(coinState)
                    str = CoinButton::coinImageList[0]; // 金币
                else
                    str = CoinButton::coinImageList[CoinButton::coinImageList.size()-1]; // 银币

                CoinButton *coin = new CoinButton(str, label);

                coin->setStyleSheet("QPushButton {border: 0;}");
                coin->setGeometry(0, 0, pm.width(), pm.height());
                coin->setCoinFlag(coinState);
                coin->setIndex(i, j);
                coin->m_coinBtnPtr = &m_coinBtn;
                btns.append(coin);

                connect(coin, &CoinButton::flipComplete, this, &PlayScene::onCoinBtnFliped);
            }
            m_coinBtn.append(btns);
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QImage img;

    // 加载游戏场景背景图
    if(img.load(":/res/PlayLevelSceneBg.png")) {
        painter.drawImage(QRect(0, 0, this->width(), this->height()), img);
    }
    // 加载标题
    if(img.load(":/res/Title.png")) {
        img = img.scaled(img.width()*0.5, img.height()*0.5);
        painter.drawImage(10, 30, img);
    }
    Q_UNUSED(event);
}

void PlayScene::onCoinBtnFliped(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    for(auto& btnVec : m_coinBtn) {
        for(auto& btn : btnVec) {
             // 是否有硬币正在翻转中  是否是反面
            if(btn->isFliping() || !btn->getCoinState()) {
                return;
            }
        }
    }

    // 游戏胜利处理
    for(auto& btnVec : m_coinBtn) {
        for(auto& btn : btnVec) {
            // 信号槽断开连接
            disconnect(btn, &CoinButton::clicked, btn, &CoinButton::coinBtnClicked);
        }
    }

    // 胜利音效
    winSound->play();

    // 胜利图片显示
    QPropertyAnimation *animation = new QPropertyAnimation(winLable, "geometry", this);
    // 设置动画间隔
    animation->setDuration(800);

    // 设置起始位置
    animation->setStartValue(QRect(winLable->x(), winLable->y(), winLable->width(), winLable->height()));

    // 设置结束位置
    animation->setEndValue(QRect(winLable->x(), winLable->y()+120, winLable->width(), winLable->height()));

    // 设置跳跃曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
