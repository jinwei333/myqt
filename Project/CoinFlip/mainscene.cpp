#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPixmap>
#include <mybutton.h>
#include <QDebug>
#include <QTimer>
#include <QSound>
#include "coinbutton.h"

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 初始化硬币翻转图片列表
    for(int i = 1; i <= 8; i++) {
        CoinButton::coinImageList.append(QString(":/res/Coin000%1.png").arg(i));
    }

    // 设置窗口尺寸
    this->setFixedSize(320, 588);
    // 设置窗口标题
    this->setWindowTitle("CoinFlip");
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    levelScene = new ChooseLevelScene(this);
//    levelScene->hide();
    connect(levelScene, &ChooseLevelScene::backBtnClicked, [=](){
        this->setGeometry(levelScene->geometry());
        this->show();
        levelScene->hide();
    });

    // 创建开始按钮
    MyButton *startBtn = new MyButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move((this->width() - startBtn->width())*0.5, this->height()*0.7);
    connect(startBtn, &MyButton::clicked, [=]() {

        QSound::play(":/res/TapButtonSound.wav");

        levelScene->setGeometry(this->geometry());

        startBtn->zoomDown();
        startBtn->zoomUp();

        // 延时进入第二场景
        QTimer::singleShot(200, [=](){
            qDebug() << "进入第二场景";
            levelScene->show();
            this->hide();
        });

        levelScene->show();
        this->hide();
    });
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QImage img;

    // 加载游戏场景背景图
    if(img.load(":/res/PlayLevelSceneBg.png")) {
        painter.drawImage(QRect(0, 0, this->width(), this->height()), img);
    }
    // 加载标题
    if(img.load(":/res/Title.png")) {
        // 缩放图片
        img = img.scaled(img.width()*0.5, img.height()*0.5);
        painter.drawImage(10, 30, img);
    }

    Q_UNUSED(event);
}

