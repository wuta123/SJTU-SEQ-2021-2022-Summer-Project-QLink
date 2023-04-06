#include "singlemode.h"
#include <iostream>
#include <QTimerEvent>
#include <QPainter>
#include <QLabel>
#include <QWidget>
#include <QTime>
#include <QFileInfo>
#include "block.h"
#include <stdio.h>
#include <QMainWindow>
#include "returnbutton.h"
#include <QPushButton>
#include <QBitmap>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>
#include "ui_singlemode.h"
#include <QFontDatabase>
#include "clock.h"
#include <QPropertyAnimation>
#include <cmath>
#include <QFile>
#include <cstring>
#include <string>
#include <QMessageBox>
#include <QTextStream>
/*本类为游戏的主窗体，所有归属于singleMode的类和变量都在该类中得到妥善的处理*/

SingleMode::SingleMode(QWidget *parent, int mode) :
    QMainWindow(parent),ui(new Ui::SingleMode)
{/*窗体的构造函数，除了定义窗体的外观之外，也创建了基于窗体的QGraphicsScene视图，调用了添加障碍物的addBar,
也对ui中的组件进行了进一步的初始化，同时调用addPlayer、addBoss、addBlocks完善了
游戏中所存在的元素，setRules函数规定了游戏中一些组件(如时钟等)的运行准则*/
/*对于从文件恢复的游戏，构造函数会调用在类末尾定义的loadGame函数以恢复游戏状态*/
    setFont(QFont("Microsoft Yahei", 9));
    setWindowTitle(QStringLiteral("连连看(单人模式)"));
    setWindowIcon(QIcon(":/icon/1x1/darkzaitzev/chameleon-glyph.svg"));
    ui->setupUi(this);
    if(mode != 4)
        this->mode = mode;
    ui->pauseWidget->hide();
    background = new QPixmap(":/image/image/The_Game_Background.png");
    QGraphicsPixmapItem *backGround = new QGraphicsPixmapItem(*background);
    scene = new QGraphicsScene(0,0,1600,900);
    scene->addItem(backGround);
    ui->graphicsView->setScene(scene);
    addBar();
    connectChara();
    connect(ui->clock, &Clock::isPaused, this, &SingleMode::pause);
    ui->backGround->setScaledContents(true);
    ui->statusFrame->setAutoFillBackground(true);
    ui->statusFrame->setPalette(QPalette(Qt::white));
    step = player.step;
    ui->HPBar->setStyleSheet("QProgressBar{background:black;} QProgressBar::chunk{background:red}");
    ui->MPBar->setStyleSheet("QProgressBar{background:black;} QProgressBar::chunk{background:blue}");
    ui->BossHp->setStyleSheet("QProgressBar{background:black;} QProgressBar::chunk{background:red}");
    addBoss();
    addBlocks();
    setRules();
    ui->blockLabel->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    ui->MarksLabel->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    ui->MarksLabel->setAlignment(Qt::AlignCenter);
    ui->finalSkillLabel->hide();
    if(mode != 4){
    ui->MarksLabel->setText("0");
    this->mode = mode;
    ui->BossHp->setMaximum(175 + 50*(mode-1));
    ui->BossHp->setValue(ui->BossHp->maximum());
    boss->setHp(ui->BossHp->maximum());
    }
    else{
        loadGame();
    }
}

SingleMode::~SingleMode()
{

}

void SingleMode::connectChara(){
    /*对角色选择子窗口发出的信号进行处理*/
    connect(ui->cSelection, &charaSelection::chara1Selected, this, [=](){
        player.loadingChara(0);
        loadingStatus(0);
    });
    connect(ui->cSelection, &charaSelection::chara2Selected, this, [=](){
        player.loadingChara(1);
        loadingStatus(1);
    });
    connect(ui->cSelection, &charaSelection::chara3Selected, this, [=](){
        player.loadingChara(2);
        loadingStatus(2);
    });
    connect(ui->cSelection, &charaSelection::chara4Selected, this, [=](){
        player.loadingChara(3);
        loadingStatus(3);
    });
}

void SingleMode::setMode(int mode)
{
    /*设定游戏的难度,mode = 1\2\3分别对应简单、普通以及困难难度，
     * mode=4表示这场游戏是从文件中恢复而来的，难度设定会继承上一次游戏设定*/
    this->mode = mode;
}

void SingleMode::pause()
{
    /*在游戏暂停时，对游戏中的各个元素以及计时器做暂停处理
     */
    paused = true;
    boss->pause();
    losingWaterTimer.stop();
    ui->pauseWidget->show();
    for(int i = 0; i<12; i++){
        blocks[i] -> pause();
    }
    FlashTimer->stop();
    lightTimer->stop();
    aThirTimer.stop();
    aDamTimer.stop();
    drinkingTimer.stop();
}

void SingleMode::mute()
{
    /*静音*/
    if(volumeIsOn){
        if(!fighting1->isFinished())fighting1->stop();
        if(!fighting2->isFinished())fighting2->stop();
        if(!fighting3->isFinished())fighting3->stop();
        volumeIsOn = false;
        fighting1Timer->stop();
        fighting2Timer->stop();
        boss->mute();
        for(int i = 0; i < 12; i++)
            blocks[i] -> mute();
    }
}

void SingleMode::unmute()
{
    /*开启声音*/
    if(!volumeIsOn){
        volumeIsOn = true;
        fighting1->setLoops(1);
        fighting1->play();
        fighting1Timer->start(118000);/*一个很丑陋的处理，因为并不知道怎么让音乐连贯*/
        boss->unmute();
        for(int i = 0; i < 12; i++)
            blocks[i] -> unmute();
    }
}

void SingleMode::startProgress()
{
    /*从暂停中恢复后会调用此函数，将场景中的所有元素恢复原本的运行状态*/

    if(ui->BossWords->isHidden())paused = false;
    ui->clock->restart();
    losingWaterTimer.start();
    for(int i = 0; i<12; i++){
        blocks[i] -> startLife();
    }
    if(ableToFlash) FlashTimer->start();
}

void SingleMode::paintEvent(QPaintEvent *event)
{
    /*留空，避免update对界面进行误操作*/
}

void SingleMode::loadingStatus(int number)
{
    /*获取到角色状态之后想场景中添加角色*/
    switch (number) {
        case 0:
            smallChara = new QPixmap(":/chara/character/cloudTheWarrior_01.png");
            break;
        case 1:
            smallChara = new QPixmap(":/chara/character/darkWarrior_01.png");
            break;
        case 2:
            smallChara = new QPixmap(":/chara/character/redHeadBandKnight_01.png");
            break;
        case 3:
            smallChara = new QPixmap(":/chara/character/thief_01.png");
            break;
        default:
            break;
    }

    ui->smallChara->setIcon(QIcon(*smallChara));
    if(firstTime)player.setPos(775,410);
    scene->addItem(&player);
}

void SingleMode::keyPressEvent(QKeyEvent *event)
{
    /*重点程序，键盘响应函数，分别对按键为上下左右键、以及z键进行处理*/
    if(paused) return;
    QPoint *currentPos;
    QPoint *newPos = new QPoint(player.x(),player.y());
    currentPos = new QPoint(player.x(),player.y());
    bool closeToWater = false;
    if(event -> key() == Qt::Key_Right){
            if(player.x()+step+player.width <= 1344) newPos = new QPoint(player.x()+step,player.y());
            else {
                newPos = new QPoint(1344-player.width,player.y());
                if(!MPStricted)closeToWater = true;
            }
            player.setCurrentDirection(1);
            player.update();
            if(Walking->isFinished() && volumeIsOn){Walking->setLoops(1);
            Walking->play();}//如果音量开启，在角色移动时播放音效
    }
    if(event -> key() == Qt::Key_Left){
            if(player.x() - step >= 269) newPos = new QPoint(player.x()-step,player.y());
            else {
                newPos = new QPoint(269,player.y());
                if(!MPStricted)closeToWater = true;
            }
            player.setCurrentDirection(2);
            player.update();
            if(Walking->isFinished() && volumeIsOn){Walking->setLoops(1);
            Walking->play();}
    }
    if(event -> key() == Qt::Key_Up){
            if(player.y()-step >= 56) newPos = new QPoint(player.x(), player.y()-step);
            else {
                newPos = new QPoint(player.x(), 56);
                if(!MPStricted)closeToWater = true;
            }
            player.setCurrentDirection(3);
            player.update();
            if(Walking->isFinished() && volumeIsOn){Walking->setLoops(1);
            Walking->play();}
    }
    if(event -> key() == Qt::Key_Down){
            if(player.y()+player.height + step<= 825) newPos = new QPoint(player.x(), player.y()+step);
            else {
                newPos = new QPoint(player.x(),825-player.height);
                if(!MPStricted)closeToWater = true;
            }
            player.setCurrentDirection(4);
            player.update();
            if(Walking->isFinished() && volumeIsOn){Walking->setLoops(1);
            Walking->play();}
    }
    player.setPos(*newPos);
    if(player.collidesWithItem(upBar)||player.collidesWithItem(downBar)||player.collidesWithItem(leftBar)||player.collidesWithItem(rightBar))
            player.setPos(*currentPos);//移动之后进行判定，如果角色的newPos与障碍物相接触，判定为移动非法，会撤销角色本来进行的移动
    //这样写是考虑到角色的移动动画能够正常地播放

    judgeCollide(closeToWater);//如果角色走到地图边缘，按照游戏规则会恢复MP值，因此调用该函数来为角色恢复MP
    //该函数同时用于判定角色所处的场地特性。
    if(event->key() == Qt::Key_Z){
        judgeEliminate();//当角色试图捡起一个物品之后的处理函数
    }
}

void SingleMode::judgeCollide(bool closeToWater)
{
    if(!paused){
    if(player.collidesWithItem(areaForDamage)) {
        if(!aDamTimer.isActive())
            aDamTimer.start();
    }else aDamTimer.stop();
    /*角色进入熔岩区，生命值开始下降*/
    if(player.collidesWithItem(areaForHealing)) {
        if(!aHealTimer.isActive())
            aHealTimer.start();
    }else aHealTimer.stop();
    /*角色走进家园区，开始慢慢回复生命值*/
    if(player.collidesWithItem(areaForThirsty)) {
        if(faster && step > 4) step = 6;
        else if(step > 4)
            step = 4;
        if(!aThirTimer.isActive())
            aThirTimer.start();
    }else {
        aThirTimer.stop();
        if(step != 2 && !faster)step = player.step;
        else if(step != 3 && faster) step = player.step+((mode == 1) ? 3:2);
    }
    /*角色走进沙漠区域，速度下降，MP值开始加速下降*/
    if(closeToWater) {
        if(!drinkingTimer.isActive())
            drinkingTimer.start();
        if(ui->MPBar->value() >= 0)
            alreadyThirstyTimer.stop();
    }else drinkingTimer.stop();
    /*如果角色MP为满，就保持MP为满的状态，如果角色之前的状态为干渴状态，就停止干渴状态对HP的损耗
     * 即停止alreadyThirstyTimer,同时MP开始慢慢回升*/
    }
}

void SingleMode::judgeEliminate()
{
    /*角色按下Z试图捡起物品之后的逻辑判断函数*/
    int i = 0;
    for(; i < 12; i++){
        if(blocks[i]->isVisible() && (abs(player.y()-(blocks[i]->y())) <= 35) && (abs(player.x()-blocks[i]->x()) <= 22))
            break;
    }//首先判断在角色周围一定范围内有没有可见物品，如果没有就会直接退出函数

    if(i != 12){
       if(carriedCode != 99 && carriedState == blocks[i]->checkState()){
            if(blocks[carriedCode] -> checkReverse())
               blocks[carriedCode]->beenMerged();
            else blocks[i]->beenMerged();
            blocks[i]->disappear();
            blocks[carriedCode]->disappear();
            carriedCode = 99;
            carriedState = 99;
            ui->blockLabel->clear();
            if(volumeIsOn){
            takeItem->setLoops(1);
            takeItem->play();}
            /*通过角色自己已经持有的道具的state和新捡起道具的state的比较
             * 确定此时可以融合，产生效果。
             * 检查boss技能是否作用于这两个方块中的一个或两个方块，如果有方块处于reversed状态，
             * 触发反效果。在融合完成后将carriedcode和carriedstate重置为99。
             * 最后播放捡起物品的音效*/
        }
        else{
           if(carriedCode == 99){
               carriedState = blocks[i]->checkState();
               carriedCode = i;
               ui->blockLabel->setPixmap(blocks[i]->getPix());
               blocks[i]->beenCarried();
               if(volumeIsOn){
               takeItem->setLoops(1);
               takeItem->play();}
               /*carriedCode=99说明当前并未捡起物品，因此将当前所捡起的物品放进背包
                * 更新carriedcode为当前物品的index，播放音效*/
           }
           else if(carriedCode != 99){
               /*如果carriedCode不为99，说明两个物品的state不一致，匹配失败
                * 两个物品直接消除，清空背包，播放捡起物品的音效*/
               ui->blockLabel->clear();
               blocks[i]->disappear();
               blocks[carriedCode]->disappear();
               carriedCode = 99;
               carriedState = 99;
               if(volumeIsOn){
               takeItem->setLoops(1);
               takeItem->play();}
           }
       }
    }
}

void SingleMode::addBar()
{
    /*添加障碍物和区域：leftBar~downBar为地图中不可越过的障碍，而areaForDamage、areaForHealing\areaForThirsty为
     * 地图中的特殊区域，进入后有不同的效果，设定好pos后将上述元素添加进入scene里*/
    leftBar = new QGraphicsPixmapItem(QPixmap(":/banner/banner/left_1.png"));
    rightBar = new QGraphicsPixmapItem(QPixmap(":/banner/banner/right_1.png"));
    upBar = new QGraphicsPixmapItem(QPixmap(":/banner/banner/up_1.png"));
    downBar = new QGraphicsPixmapItem(QPixmap(":/banner/banner/down_1.png"));

    scene -> addItem(leftBar);
    scene -> addItem(rightBar);
    scene -> addItem(upBar);
    scene -> addItem(downBar);

    leftBar -> setPos(318,436);
    rightBar -> setPos(837,436);
    upBar -> setPos(758,136);
    downBar -> setPos(758,507);

    areaForDamage = new QGraphicsPixmapItem(QPixmap(":/banner/banner/AreaForDamage.png"));
    areaForHealing = new QGraphicsPixmapItem(QPixmap(":/banner/banner/AreaForHealing.png"));
    areaForThirsty = new QGraphicsPixmapItem(QPixmap(":/banner/banner/AreaForThirsty.png"));
    scene -> addItem(areaForDamage);
    scene -> addItem(areaForHealing);
    scene -> addItem(areaForThirsty);

    areaForDamage -> setPos(277,91);
    areaForThirsty -> setPos(828,91);
    areaForHealing -> setPos(828,456);
    areaForDamage->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    areaForThirsty->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    areaForHealing->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    /*通过修改判断逻辑，让判断变得更加精确*/
}

void SingleMode::setClocks()
{
    /*该函数主要设定了游戏中的一些时钟以及它们的逻辑处理*/
    aDamTimer.setInterval(750);
    aHealTimer.setInterval(2000);
    aThirTimer.setInterval(1200);
    alreadyThirstyTimer.setInterval(1000);
    losingWaterTimer.setInterval(2000);
    drinkingTimer.setInterval(200);

    connect(&aDamTimer, &QTimer::timeout, [=](){
        if(!paused){
        ui->HPBar->setValue(decreaseHp(2));
        }
    });
    connect(&aHealTimer, &QTimer::timeout,[=](){
        if(!paused){
        int a = ui->HPBar->value();
        if(a+1 < ui->HPBar->maximum())
            ui->HPBar->setValue(a+1);
        else ui->HPBar->setValue(100);
        }
    });
    connect(&aThirTimer, &QTimer::timeout, [=](){
        if(!paused){
        ui->MPBar->setValue(decreaseMp(2));
        }
    });
    connect(&alreadyThirstyTimer, &QTimer::timeout, [=](){
        if(!paused){
        ui->HPBar->setValue(decreaseHp(2));
        }
    });
    connect(&drinkingTimer, &QTimer::timeout, [=](){
        if(!paused && ableToDrink){
        if(drinking->isFinished() && volumeIsOn){
            drinking->setLoops(1);
            drinking->play();
        }
        int a = ui->MPBar->value();
        if(a+2 < ui->MPBar->maximum()) ui->MPBar->setValue(a+2);
        else ui->MPBar->setValue(100);
        }
    });
    connect(&losingWaterTimer, &QTimer::timeout, [=](){
        if(!paused){
        ui->MPBar->setValue(decreaseMp(1));
        }
    });
    PaceFastTimer = new QTimer;
    PaceFastTimer->setInterval(15000);
    lightTimer = new QTimer;
    lightTimer->setInterval(1000);
    FlashTimer = new QTimer;
    FlashTimer->setInterval(1000);
    connect(FlashTimer, &QTimer::timeout, this, [=](){
        if(flashCount >= 0) flashCount--;
        else{
            FlashTimer->stop();
            ui->statusLabel->setText(u8"末影之眼的效力已经过去了...");
            takeItem->setLoops(1);
            takeItem->play();
            ableToFlash = false;
            flashCount = 5;
        }
    });
    connect(PaceFastTimer, &QTimer::timeout, this, [=]{
        PaceFastTimer->stop();
        faster = false;
    });
    connect(lightTimer, &QTimer::timeout, this, [=]{
        if(lightTime > 0)
        {
            lightTime--;
            lightBlock();
        }else{
            lightTimer->stop();
            lightTime = 15;
            for(int i = 0; i < 12; i++)
                blocks[i]->shadow();
        }
    });
}

void SingleMode::addBlocks()
{
    /*用于在游戏刚开始的时候添加方块，并且设定好所有方块对主窗口的作用效果*/
    srand(time(NULL));
    int i = 0;
    for(; i < 12; i++){
         if(i < 6)
            blocks[i] = new Blocks(mode);
         else
            blocks[i] = new Blocks(*(blocks[(i+6) % 12]));
         connect(blocks[i], &Blocks::causeEffects, [=](int effect){
             if(blocks[i]->checkReverse()){
             ui->statusLabel->setText(u8"你所触碰的物件已经被sans动了手脚...");
             switch (effect) {
                case 0:
                 ui->HPBar->setValue(decreaseHp(15));
                 break;
                case 1:
                 ui->HPBar->setValue(decreaseHp(30));
                 break;
                case 2:
                 boss->selfRecoverSlot();
                 ui->BossHp->setValue(boss->checkHp());
                 ui->HPBar->setValue(decreaseHp(10));
                 break;
                case 3:
                 ui->MPBar->setValue(decreaseMp(15));
                 ui->HPBar->setValue(decreaseHp(10));
                 break;
                case 4:
                 ui->HPBar->setValue(decreaseHp(15));
                 break;
                case 6:
                 ui->HPBar->setValue(decreaseHp(10));
                 ui->MPBar->setValue(decreaseMp(10));
                 break;
                case 7:case 8:case 5:case 9:
                 ui->HPBar->setValue(decreaseHp(10));
             default:
                 break;
                }
             }
             else
             switch (effect) {
                case 0:
                 hit();
                 break;
                case 1:
                 specialHit();
                 break;
                case 2:
                 HPBster();
                 break;
                case 3:
                 MPBster();
                 break;
                case 4:
                 speedBster();
                 break;
                case 5:
                 hint();
                 break;
                case 6:
                 high();
                 break;
                case 7:
                 shuffle();
                 break;
                case 8:
                 flash();
                 break;
                case 9:
                 addTime();
                 break;
             default:
                 break;
             }
         });
    }
    for(i = 0; i < 12; i++){
        scene -> addItem(blocks[i]);
        blocks[i]->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    }
}

void SingleMode::mousePressEvent(QMouseEvent *event){
    if(!ableToFlash) return;
    if(event -> buttons() & Qt::LeftButton){
        QPointF newPos = event->pos();
        QPointF currentPos = player.pos();
        if(newPos.x()+player.width <= 1344 && newPos.x() >= 269
           && newPos.y() >= 56 && newPos.y()+player.height <= 825){
            player.setPos(newPos);
            if(player.collidesWithItem(upBar)||player.collidesWithItem(downBar)||player.collidesWithItem(leftBar)||player.collidesWithItem(rightBar))
                    player.setPos(currentPos);
            else{
                FlashTimer->stop();
                ableToFlash = false;
                flashCount = 5;
                takeItem->setLoops(1);
                takeItem->play();
                return;
            }
        }
    }
}

void SingleMode::shuffle()
{
    /*shuffle道具的实现*/
    marks += 20;
    ui->MarksLabel->setText(QString::number(marks));
    for(int i = 0; i < 12; i++)
        blocks[i]->disappear();
    ui->statusLabel->setText(u8"道具全体重生啦！");
    ui->BossHp->setValue(boss->reduceLife(20));
}

void SingleMode::hint()
{
    /*hint道具的实现*/
    marks += 15;
    ui->MarksLabel->setText(QString::number(marks));
    lightTime = 15;
    ui->BossHp->setValue(boss->reduceLife(20));
    lightBlock();
    if(lightTimer->isActive()) lightTimer->stop();
    lightTimer->start();
    ui->statusLabel->setText(u8"多年的近视怎么好了？");
}

void SingleMode::speedBster()
{
    /*speedBster道具的实现*/
    marks += 25;
    ui->MarksLabel->setText(QString::number(marks));
    ui->statusLabel->setText(u8"啊！是耐克的运动鞋！");
    ui->BossHp->setValue(boss->reduceLife(15));
    step = step + ((mode == 1) ? 5 : 4);
    faster = true;
    PaceFastTimer->start();
}

void SingleMode::HPBster()
{
    /*HPBster道具的实现*/
    marks += 20;
    ui->MarksLabel->setText(QString::number(marks));
    ui->BossHp->setValue(boss->reduceLife(15));
    if(ui->HPBar->value()+((mode == 1) ? 35:25) < 100)
       ui->HPBar->setValue(ui->HPBar->value()+((mode == 1) ? 35:25));
    else ui->HPBar->setValue(100);
    ui->statusLabel->setText(u8"你喝下的魔药让你的内心得到温暖。");
    if(volumeIsOn){
    drinking -> setLoops(1);
    drinking -> play();}
}

void SingleMode::MPBster()
{
    /*MPBster道具的实现*/
    marks += 20;
    ui->MarksLabel->setText(QString::number(marks));
    ui->BossHp->setValue(boss->reduceLife(15));
    if(ui->MPBar->value()+((mode == 1) ? 45:30) < 100)
       ui->MPBar->setValue(ui->MPBar->value()+((mode == 1) ? 45:30));
    else ui->MPBar->setValue(100);
    ui->statusLabel->setText(u8"你喝下的魔药缓解了你的干渴。");
    if(alreadyThirstyTimer.isActive()) alreadyThirstyTimer.stop();
    if(volumeIsOn){
    drinking -> setLoops(1);
    drinking -> play();}
}

void SingleMode::high()
{
    /*superItem道具的实现*/
    marks += 100;
    ui->MarksLabel->setText(QString::number(marks));
    ui->statusLabel->setText(u8"我真是high到不行啦！！！");
    ui->BossHp->setValue(boss->reduceLife(60));
    if(ui->HPBar->value()+((mode == 1) ? 25:15) < 100)
       ui->HPBar->setValue(ui->HPBar->value()+((mode == 1) ? 25:15));
    else ui->HPBar->setValue(100);
    if(ui->MPBar->value()+((mode == 1) ? 35:20) < 100)
       ui->MPBar->setValue(ui->MPBar->value()+((mode == 1) ? 35:20));
    else ui->MPBar->setValue(100);
    step = step + ((mode == 1) ? 5 : 4);
    faster = true;
    PaceFastTimer->start();
    if(alreadyThirstyTimer.isActive()) alreadyThirstyTimer.stop();
}

void SingleMode::hit()
{
    /*普通攻击*/
    marks += 5+ui->MPBar->value()/10;
    ui->MarksLabel->setText(QString::number(marks));
    ui->BossHp->setValue(boss->reduceLife(5+ui->MPBar->value()/10));
    ui->statusLabel->setText(u8"你对sans发动了技能[普通拳]");
}

void SingleMode::flash(){
    /*闪现*/
    marks += 15;
    ui->MarksLabel->setText(QString::number(marks));
    ui->BossHp->setValue(boss->reduceLife(15));
    ui->statusLabel->setText(u8"你获得了末影之眼，在5秒之内可以闪现到地图中的任意位置，当然啦，前提是位置合理");
    ableToFlash = true;
    FlashTimer->start();
}

void SingleMode::addTime(){
    /*加时*/
    marks += 15;
    ui->MarksLabel->setText(QString::number(marks));
    ui->BossHp->setValue(boss->reduceLife(15));
    ui->statusLabel->setText(u8"剩余的时间变多了...");
    ui->clock->setRemainTime(ui->clock->remainTime()+6);
}

void SingleMode::specialHit()
{
    /*specialHit道具的实现*/
    marks += 30+4*ui->MPBar->value()/10;
    ui->MarksLabel->setText(QString::number(marks));
    ui->BossHp->setValue(boss->reduceLife(30+4*ui->MPBar->value()/10));
    ui->statusLabel->setText(u8"你对sans发动了技能[连续普通拳]");
}

void SingleMode::lightBlock()
{
    /*hint道具的操作函数，调用该操作函数需要经过判断函数judgeLightNeed的确认方可进行*/
   if(judgeLighintNeed()){
        bool ok = false;
        for(int i = 0; i < 12; i++){
            for(int j = i+1; j < 12; j++){
                if(blocks[i]->checkState()==blocks[j]->checkState() && blocks[i]->returnCount() && blocks[j]->returnCount() &&
                        !blocks[i]->checkLighted()&&!blocks[j]->checkLighted()&& !blocks[i]->merging() && !blocks[j]->merging()){
                    blocks[i]->beenLighted();
                    blocks[j]->beenLighted();
                    ok = true;
                    break;
                }
            }
           if(ok) break;
        }
   }
   currrentLighted = 2;
}

bool SingleMode::judgeLighintNeed()
{
    /*该函数的逻辑是首先判断当前场上所有的可见高亮道具数目，如果
     * 少于两个，自动寻找下一对匹配的道具，进行高亮操作
     * 始终确保在hint道具生效期内，场上可见的高亮道具为两个*/
    currrentLighted = 0;
    for(int i = 0; i < 12; i++){
        if(blocks[i]->checkLighted() && blocks[i]->isVisible())
            currrentLighted++;
    }
    if(currrentLighted != 2){
        for(int i = 0; i < 12; i++)
            blocks[i]->shadow();
        return true;
    }
    return false;
}

void SingleMode::addBoss()
{
    /*添加Boss的贴图，以及一些相关的操作，添加boss对话框bosswords，
    以及对BossWords的信号做出反馈*/
    boss = new Boss(nullptr, this->mode);

    ui->BossWords->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    ui->BossName->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    int EnglishFont_id = QFontDatabase::addApplicationFont(":/font/font/English.ttf");
    int ChineseFont_id = QFontDatabase::addApplicationFont(":/font/font/Chinese.ttf");
    QFontDatabase base;
    QStringList qsl = base.families();

    QFont EnglishFont(qsl[EnglishFont_id]);
    EnglishFont.setPointSize(20);
    QFont ChineseFont(qsl[ChineseFont_id]);
    ChineseFont.setPointSize(20);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    ui->BossName->setPalette(pa);
    ui->BossName->setFont(EnglishFont);
    ui->BossName->setText("sans");
    ui->BossName->setAlignment(Qt::AlignCenter);
    ui->BossWords->setPalette(pa);
    ui->BossWords->setFont(ChineseFont);
    ui->BossWords->setText("......");

    connect(ui->BossWords, &WordsOfBoss::over, this, [=](){
                ui->BossName->hide();
                ui->BossFace->hide();
                ui->clock->restart();
                boss->resume();
                startProgress();
    });
    connect(ui->BossWords, &WordsOfBoss::finalOver, this, [=](){
                ui->BossName->hide();
                ui->BossFace->hide();
                ui->clock->restart();
                boss->resume();
                startProgress();
    });

    connect(boss, &Boss::beenDefeated, this, &SingleMode::showVictory);
    connect(ui->pauseWidget, &PauseWidget::resume, boss, &Boss::resume);
    connect(boss, &Boss::sayWords, this,[=](){
                ui->BossName->show();
                ui->BossWords->say();
                ui->BossFace->show();
    });
    boss->setPos(1450,200);
    scene->addItem(boss);
    addBossSkills();
}

void SingleMode::addBossSkills()
{
    /*实现boss的各种技能对游戏中元素的操作*/
    ui->statusLabel->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    ui->statusLabel->setFixedSize(ui->statusLabel->size());
    ui->statusLabel->setWordWrap(true);
    QFontDatabase base;
    QStringList qsl = base.families();
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    ui->statusLabel->setPalette(pa);

    connect(boss, &Boss::restrictMpRecover, this, [=](){
        if(!paused){
        showShaken();
        ui->statusLabel->setText(u8"sans暂时让你失去了喝水的能力。。持续"+QString::number(5*mode)+u8"秒钟");
        MPStricted = true;
        MPStrictTimer = new QTimer;
        MPStrictTimer->start(5000*mode);
        ableToDrink = false;
        connect(MPStrictTimer, &QTimer::timeout, [=](){
            ableToDrink = true;
            ui->statusLabel->setText(u8"你又可以喝水了。。。");
            MPStricted = false;
            MPStrictTimer->stop();
        });
        }
    });
    connect(boss, &Boss::slowPace, this, [=](){
        if(!paused){
        showShaken();
        ui->statusLabel->setText(u8"sans暂时让你的速度减慢了。。持续"+QString::number(4+2*(mode-1))+u8"秒钟");
        if(!faster)step = 2;
        else step = 3;
        PaceSlowTimer = new QTimer;
        PaceSlowTimer->start(4000+2000*(mode-1));
        connect(PaceSlowTimer, &QTimer::timeout, [=](){
            ui->statusLabel->setText(u8"你又可以自由行走了。。。");
            if(!faster)step = player.step;
            else step = player.step + ((mode == 1) ? 3:2);
            PaceSlowTimer -> stop();
        });
        }
    });
    connect(boss, &Boss::selfRecover, this, [=]{
        if(!paused){
        showTransparent();
        ui->statusLabel->setText(u8"sans运用魔力，回复了生命值。");
        boss->selfRecoverSlot();
        ui->BossHp->setValue(boss->checkHp());
        }
    });
    connect(boss, &Boss::playerLifeReduce, this, [=](){
        if(!paused){
        showShaken();
        ui->statusLabel->setText(u8"sans轻轻地拍了你一下，你损失了"+QString::number(10+5*mode)+u8"点生命值");
        ui->HPBar->setValue(decreaseHp(10+5*mode));
        }
    });
    connect(boss, &Boss::fakeItem, this, [=](){
        if(!paused){
        showTransparent();
        int k = 0;
        for(int i = 0; i <= 12; i++){
            if(blocks[i]->isVisible() && !blocks[i]->checkReverse()){
                blocks[i]->beenReversed();
                k++;
            }
            if(k == 3) break;
        }
        ui->statusLabel->setText(u8"sans悄悄地对3个道具做了一些手脚。。。");
        }
    });
}

void SingleMode::checkHp()
{
    if(ui->HPBar->value() <= 0)
        showFail();
}

void SingleMode::setRules()
{
    /*进行一些游戏设定，包括BGM、时钟以及窗体外观等*/
    fighting1 -> setLoops(1);
    fighting2 -> setLoops(1);
    fighting3 -> setLoops(-1);
    fighting1 -> play();
    fighting1Timer = new QTimer;
    fighting2Timer = new QTimer;
    fighting1Timer->start(118000);
    connect(fighting1Timer, &QTimer::timeout,this, [=]{
        if(fighting1->isFinished()){
        fighting2->play();
        fighting2Timer->start(58000);
        fighting1Timer->stop();
        }
    });
    connect(fighting2Timer, &QTimer::timeout, this, [=](){
        if(fighting2->isFinished() && fighting1->isFinished()){
        fighting3->play();
        fighting2Timer->stop();
        }
    });
    connect(ui->pauseWidget, &PauseWidget::turnOnVolumn, this, &SingleMode::unmute);
    connect(ui->pauseWidget, &PauseWidget::turnOffVolumn, this, &SingleMode::mute);
    connect(ui->pauseWidget, &PauseWidget::resume, this, &SingleMode::startProgress);
    connect(ui->pauseWidget, &PauseWidget::returnToBackground, this, [=](){
        fighting1->stop();
        fighting2->stop();
        fighting3->stop();
        delete fighting1Timer;
        delete fighting2Timer;
        this->close();
        emit returnToHomepage();
    });
    connect(ui->pauseWidget, &PauseWidget::savegame, [=]{
        QMessageBox::about(NULL,u8"保存游戏", u8"游戏已保存！");
        saveGame();
    });
    connect(ui->clock, &Clock::timesUp, this, [=](){
        ui->clock->pause();
        showFail();
    });
    setClocks();
}

void SingleMode::showFail()
{
    /*战斗失败之后调出结算画面，并停止当前的游戏，播放战败动画，
     * 以及显示分数，将该模式胜利的参数写入文件，为玩家解锁下一个难度*/
      paused = true;
      pause();
      boss->pause();
      boss->stop();
      ui->clock->pause();
      for(int i = 0; i<12; i++){
          blocks[i] -> pause();
      }
      step = player.step = 0;
      if(boss->isAlive()){
         boss -> stop();
         boss -> deliverFinalSkill();
         ui->HPBar->setValue(0);
         QMovie *movie = new QMovie(":/image/boss/Boss_FinalSkill.gif");
         ui -> finalSkillLabel -> setMovie(movie);
         ui -> finalSkillLabel -> show();
         movie -> start();
         connect(movie, &QMovie::frameChanged,[=](int frameNumber){
             if (frameNumber == movie->frameCount() - 1) {
             movie->stop();
             delete  movie;
             ui -> finalSkillLabel -> hide();
             }
         });
      }
      mute();
      ui->pauseWidget->setMarksForFailure(marks);
}

void SingleMode::showVictory()
{
    /*战斗胜利之后调出结算画面，并停止当前的游戏，
     * 以及显示分数*/
    paused = true;
    pause();
    boss->pause();
    ui->clock->pause();
    for(int i = 0; i<12; i++){
        blocks[i] -> pause();
    }
    step = player.step = 0;
    mute();
    ui->pauseWidget->setMarksForVictory(marks);
    if(mode != 3){
        QFile *saver = new QFile("C:\\temp\\saver.txt");
        saver->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate);
        QString a = QString::number(mode+1);
        saver->write(a.toUtf8());
        saver->close();
    }
}

void SingleMode::showShaken()
{
    /*boss技能对玩家造成影响时的技能特效，窗口抖动*/
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "pos");
    pAnimation->setDuration(500);
    pAnimation->setLoopCount(2);
    pAnimation->setKeyValueAt(0, QPoint(frameGeometry().x() - 3, frameGeometry().y() - 3));
    pAnimation->setKeyValueAt(0.1, QPoint(frameGeometry().x() + 6, frameGeometry().y() + 6));
    pAnimation->setKeyValueAt(0.2, QPoint(frameGeometry().x() - 6, frameGeometry().y() + 6));
    pAnimation->setKeyValueAt(0.3, QPoint(frameGeometry().x() + 6, frameGeometry().y() - 6));
    pAnimation->setKeyValueAt(0.4, QPoint(frameGeometry().x() - 6, frameGeometry().y() - 6));
    pAnimation->setKeyValueAt(0.5, QPoint(frameGeometry().x() + 6, frameGeometry().y() + 6));
    pAnimation->setKeyValueAt(0.6, QPoint(frameGeometry().x() - 6, frameGeometry().y() + 6));
    pAnimation->setKeyValueAt(0.7, QPoint(frameGeometry().x() + 6, frameGeometry().y() - 6));
    pAnimation->setKeyValueAt(0.8, QPoint(frameGeometry().x() - 6, frameGeometry().y() - 6));
    pAnimation->setKeyValueAt(0.9, QPoint(frameGeometry().x() + 6, frameGeometry().y() + 6));
    pAnimation->setKeyValueAt(1, QPoint(frameGeometry().x() - 3, frameGeometry().y() - 3));
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void SingleMode::showTransparent()
{
    /*boss技能对场地造成影响时的技能特效，窗口闪现*/
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "windowOpacity");
    pAnimation->setDuration(1000);
    pAnimation->setKeyValueAt(0, 1);
    pAnimation->setKeyValueAt(0.5, 0);
    pAnimation->setKeyValueAt(1, 1);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

int SingleMode::decreaseHp(int value)
{
    /*集判定和操作为一体的玩家减血函数
    在主体中多次使用*/
    int a = ui->HPBar->value() - value;
    if(a > 0){
        if(volumeIsOn){
            playerBeenHurt->setLoops(1);
            playerBeenHurt->play();}
        return a;
        }
    else if(ui->HPBar->value()!=0){
        if(volumeIsOn){
            playerBeenHurt->setLoops(1);
            playerBeenHurt->play();}
        showFail();
        return 0;
    }
    else{
        return 0;
    }
}

int SingleMode::decreaseMp(int value)
{
    /*集判定和操作为一体的玩家减MP函数
    在主体中多次使用*/
    int a = ui->MPBar->value() - value;
    if(a > 0)
        return a;
    else {
        if(!alreadyThirstyTimer.isActive())alreadyThirstyTimer.start();
        return 0;
    }
}


void SingleMode::saveGame()
{
    /*保存游戏*/
    QFile *reserver = new QFile("C:\\temp\\isSaved.txt");
    QTextStream k(reserver);
    reserver->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate);
    k << "1";
    reserver->close();

    QFile *saver = new QFile("C:\\temp\\game.txt");
    saver->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate);
    QString a = QString::number(mode);
    QTextStream in(saver);
    in << QString::number(mode) << endl;
    in << QString::number(player.x())+","+QString::number(player.y())<<endl;
    in << QString::number(ui->clock->remainTime()) << endl;
    for(int i = 0; i < 12; i++){
        in << QString::number(blocks[i]->x())+","+QString::number(blocks[i]->y())+","+QString
              ::number(blocks[i]->checkState()) << "," << QString::number(blocks[i]->isVisible()) << endl;
    }
    in << QString::number(ui->BossWords->currentSequece()) +"," + QString::number(ui->BossWords->currentSentences())<<endl;
    in << QString::number(ui->HPBar->value())<<endl;
    in << QString::number(ui->MPBar->value())<<endl;
    in << QString::number(ui->BossHp->value())<<endl;
    in << QString::number(player.charaType) << endl;
    in << ui->MarksLabel->text() << endl;
    saver->close();
}


void SingleMode::loadGame()
{
    /*加载游戏恢复游戏状态*/
    firstTime = false;
    QFile *reader = new QFile("C:\\temp\\game.txt");
    reader->open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream out(reader);
    int mode;
    mode = (out.readLine().toInt());
    this -> mode = mode;
    boss -> setMode(mode);
    QString playerx, playery;
    QString playerpos = out.readLine();
    bool change = false;
    for(int i = 0; i < playerpos.length() && playerpos[i] != '\n'; i++){
        if(playerpos[i] == ','){
            change = true;
            continue;
        }
        if(change) playery.append(playerpos[i]);
        else playerx.append(playerpos[i]);
    }
    player.setPos(playerx.toInt(), playery.toInt());
    int remainTime = (out.readLine().toInt());
    ui->clock->setRemainTime(remainTime);
    QString itemPos[12];
    for(int i = 0; i < 12; i++){
        QString itemx, itemy, itemState, itemPos, itemVisible;
        int numberOfChange = 0;
        itemPos = out.readLine();
        for(int j = 0; j < itemPos.length() && itemPos[j] != '\n'; j++){
            if(itemPos[j] == ','){
                numberOfChange++;
                continue;
            }
            if(numberOfChange == 0)
                itemx.append(itemPos[j]);
            if(numberOfChange == 1)
                itemy.append(itemPos[j]);
            if(numberOfChange == 2)
                itemState.append(itemPos[j]);
            if(numberOfChange == 3)
                itemVisible.append(itemPos[j]);
        }
        blocks[i]->setPos(itemx.toInt(),itemy.toInt());
        blocks[i]->loadState(itemState.toInt());
        if(!itemVisible.toInt())
            blocks[i]->disappear();
    }
    QString theBossWords, sequence, sentence;
    theBossWords = out.readLine();
    bool change2 = false;
    for(int i = 0; i < theBossWords.length(); i++){
        if(theBossWords[i] == ',') {
            change2 = true;
            continue;
        }
        if(change2) sentence.append(theBossWords[i]);
        else sequence.append(theBossWords[i]);
    }
    ui->BossWords->loadCurrentSentences(sentence.toInt());
    ui->BossWords->loadSequence(sequence.toInt());
    int HP, MP, BossHP;
    HP = (out.readLine().toInt());
    MP = (out.readLine().toInt());
    BossHP = (out.readLine().toInt());
    ui->HPBar->setValue(HP);
    ui->MPBar->setValue(MP);
    ui->BossHp->setMaximum(175 + 50*(mode-1));
    ui->BossHp->setValue(BossHP);
    boss->setHp(BossHP);
    int charaType = out.readLine().toInt();
    ui->cSelection->hide();
    player.loadingChara(charaType);
    loadingStatus(charaType);
    if(HP <= 0) showFail();
    if(BossHP <=0) showVictory();
    QString marks = out.readLine();
    ui->MarksLabel->setText(marks);
    this->marks = marks.toInt();
}
