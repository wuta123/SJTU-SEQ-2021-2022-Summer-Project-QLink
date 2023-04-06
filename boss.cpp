#include "boss.h"

Boss::Boss(QObject *parent, int mode)
{   /*定义一些boss的贴图，以及技能的发出*/
    count = 28;
    skills[0] = QPixmap(":/image/boss/Boss_Skill1.png");
    skills[1] = QPixmap(":/image/boss/Boss_theNormalState.png");
    skills[2] = QPixmap(":/image/boss/Boss_Skill2.png");
    lifeReduceValue = 10*mode;
    slowedPace = mode;
    fakeItemNum = mode;
    selfRecoverValue = 15+mode * 5;
    maximumHP = HP = 175 + 50*(mode-1);
    connect(this, &Boss::playerLifeReduce, this, [=](){
        isDeliveringSkills = true;
        update();
    });
    connect(this, &Boss::restrictMpRecover, this, [=](){
        isDeliveringSkills = true;
        update();
    });
    connect(this, &Boss::slowPace, this, [=](){
        isDeliveringSkills = true;
        update();
    });
    connect(this, &Boss::selfRecover, this, [=](){
        isDeliveringSkills = true;
        update();
    });
    connect(this, &Boss::fakeItem, this, [=](){
        isDeliveringSkills = true;
        update();
    });
    connect(this, &Boss::finalSkill, this, [=](){
        isDeliveringSkills = true;
        update();
    });
    theTimer = new QTimer;
    theTimer -> setInterval(1000);
    skillTimer = new QTimer;
    skillTimer -> setInterval(750);
    beenHitTimer = new QTimer;
    beenHitTimer -> setInterval(500);
    connect(theTimer, &QTimer::timeout, this, &Boss::timeoutEvent);
    connect(skillTimer, &QTimer::timeout, [=](){
        if(currentFrame < 2)
            update();
        else{
            isDeliveringSkills = false;
            currentFrame = 0;
            skillTimer->stop();
        }
    });
    connect(beenHitTimer, &QTimer::timeout, [=]{
        beenHit = false;
        update();
    });
}

void Boss::timeoutEvent()
{
    /*在一定的时间boss弹出对话框以及发动技能*/
    if(count > 0) count--;
    else{
        skill();
        emit sayWords();
        count = 28;
    }
}

QRectF Boss::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void Boss::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*绘制三种不同状态的boss贴图，受击状态、发动技能状态以及通常状态*/
    if(!isDeliveringSkills && !beenHit)
        painter -> drawPixmap(boundingRect().topLeft(), QPixmap(":/image/boss/Boss_theNormalState.png"));
    else if(isDeliveringSkills){
        if(!skillTimer->isActive()) skillTimer->start();
        if(currentFrame < 2){
            painter -> drawPixmap(boundingRect().topLeft(), skills[currentFrame]);
            currentFrame++;
        }
    }else if(beenHit){
        painter -> drawPixmap(boundingRect().topLeft()+QPointF(5,5), QPixmap(":/image/boss/Boss_theHurtState.png"));
        beenHitTimer->start();
    }
}

void Boss::skill()
{
    /*发动技能，以及播放技能的音效*/
    if(!muted){
    bossSkill->setLoops(1);
    bossSkill->play();}
    srand(time(NULL));
    int skill = rand() % 10 + 1;
    switch (skill) {
        case 6:
        case 2:
            emit restrictMpRecover();
            break;
        case 4:
        case 7:
            emit slowPace();
            break;
        case 5:
        case 3:
            emit playerLifeReduce();
            break;
        case 8:
        case 9:
            emit fakeItem();
            break;
        case 10:
        case 1:
            emit selfRecover();
            break;
        default:
            emit selfRecover();
            break;
    }
}

int Boss::reduceLife(int num)
{
    /*boss受击的反馈函数*/
    if(!muted){
        bossBeenHurt->setLoops(1);
        bossBeenHurt->play();
    }
    if(HP > num){
        HP -= num;
        beenHit = true;
        update();
        return HP;
    }
    else{
        emit beenDefeated();
        beenHit = true;
        update();
        return 0;
    }
}

void Boss::pause()
{
    /*暂停游戏时调用*/
    theTimer->stop();
    count--;
}

void Boss::resume()
{
    /*从暂停恢复*/
    theTimer->start();
}

bool Boss::isAlive()
{
    return HP > 0;
}

void Boss::deliverFinalSkill()
{
    /*在最终倒计时结束之后播放技能动画*/
    emit finalSkill();
}

void Boss::selfRecoverSlot(){
    if(HP+selfRecoverValue <= maximumHP)
        HP += selfRecoverValue;
    else
        HP = maximumHP;
}

int Boss::checkHp()
{
    return HP;
}

void Boss::stop(){
    theTimer->stop();
    skillTimer->stop();
}

void Boss::mute()
{
    muted = true;
}

void Boss::unmute(){
    muted = false;
}

void Boss::setHp(int value)
{
    HP = value;
}

void Boss::setMode(int value)
{
    lifeReduceValue = 10*value;
    slowedPace = value;
    fakeItemNum = value;
    selfRecoverValue = 15+value * 5;
    maximumHP = HP = 175 + 50*(value-1);
}
