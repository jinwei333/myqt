#include "chooselevelscene.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    qDebug() << "进入第二场景";
    // 设置窗口尺寸
    this->setFixedSize(320, 588);
    // 设置窗口标题
    this->setWindowTitle("CoinFlip");
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    // 创建选关场景的返回按钮
    MyButton *backBtn = new MyButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(), this->height()-backBtn->height());

    connect(backBtn, &MyButton::clicked, [=](){
        QSound::play(":/res/BackButtonSound.wav");
        // 延时200ms，发送返回按钮点击信号
        QTimer::singleShot(100, [=](){ emit backBtnClicked(); });
    });

    // 创建关卡按钮
    for(int i = 0; i < 20; i++) {
        MyButton *levelBtn = new MyButton(":/res/LevelIcon.png");
        levelBtn->setParent(this);
        levelBtn->move(25 + (i%4)*70, 150+(i/4)*70);

        connect(levelBtn, &MyButton::clicked, [=]() {
            QSound::play(":/res/TapButtonSound.wav"); // 播放音效
            playScene = new PlayScene(i+1);
            playScene->setGeometry(this->geometry());
            playScene->show();
            this->hide();

            // 连接 第三场景返回按钮
            connect(playScene, &PlayScene::backBtnClicked, [=](){
                this->setGeometry(playScene->geometry());
                this->show();       // 选关场景显示
                playScene->hide();  // 游戏场景隐藏

                delete playScene;   // 删除游戏场景
                playScene = nullptr;
            });
        });

        // 按钮本身的Lable显示位置不对，所以自定义一个
        QLabel *label = new QLabel(levelBtn);
        label->setFixedSize(levelBtn->width(), levelBtn->height());

        QFont serifFont("Times", 10, QFont::Bold);
        label->setFont(serifFont);

        label->setAlignment(Qt::AlignCenter);
        label->setText(QString::number(i+1));
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QImage img;

    // 加载背景
    if(img.load(":/res/OtherSceneBg.png")) {
        painter.drawImage(QRect(0, 0, this->width(), this->height()), img);
    }

    // 加载标题
    if(img.load(":/res/Title.png")) {
        // 标题水平居中
        painter.drawImage((this->width() - img.width())*0.5, 30, img);
    }
    Q_UNUSED(event);
}
