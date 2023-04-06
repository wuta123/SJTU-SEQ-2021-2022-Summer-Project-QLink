#pragma once
#ifndef BOSS_H
#define BOSS_H

#include <QObject>
#include <QGraphicsItem>
#include <QPaintEvent>
#include <QPainter>
#include <QRectF>
#include <QTimer>
#include <QTimerEvent>
#include <QSound>
class Boss : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Boss(QObject *parent = nullptr, int mode = 1);// 1 == easy,  2 == normal,  3 == hard
private:
    int count;
    bool isDeliveringSkills = false;
    bool beenHit = false;
    QSound *bossBeenHurt = new QSound(":/soundEffects/music/BossBeenHurt.wav", this);
    QSound *bossSkill = new QSound(":/soundEffects/music/BossSkill.wav", this);
    QPixmap skills[3];
    int width = 150;
    int height = 196;
    int maximumHP;
    int HP;
    int currentFrame = 0;
    int lifeReduceValue;
    int slowedPace;
    int selfRecoverValue;
    int fakeItemNum;
    int accumulateDamage = 0;
    QString currentFileName;
    QTimer *theTimer;
    QTimer *skillTimer;
    QTimer *beenHitTimer;
    bool muted = false;
protected:
    void timeoutEvent();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void skill();
signals:
    void playerLifeReduce();
    void restrictMpRecover();
    void slowPace();
    void selfRecover();
    void fakeItem();
    void finalSkill();
    void sayWords();
    void beenDefeated();
public slots:
    int reduceLife(int);
    void pause();
    void resume();
    void stop();
    void mute();
    void unmute();
    void setHp(int value);
    void setMode(int value);
public:
    bool isAlive();
    void deliverFinalSkill();
    void selfRecoverSlot();
    int checkHp();
};

#endif // BOSS_H
