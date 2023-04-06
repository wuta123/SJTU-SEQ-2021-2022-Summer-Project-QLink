#pragma once
#ifndef SINGLEMODE_H
#define SINGLEMODE_H

#include <QMainWindow>
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QLabel>
#include <QTime>
#include <QMouseEvent>
#include "returnbutton.h"
#include <charaselection.h>
#include "player.h"
#include <QProgressBar>
#include <QDockWidget>
#include <QMouseEvent>
#include "clock.h"
#include "block.h"
#include "boss.h"
#include <QMovie>
#include <QSound>
#include <QMouseEvent>

namespace Ui {
    class SingleMode;
}



class SingleMode : public QMainWindow
{
    Q_OBJECT
public:
    void addMark(int num);
    explicit SingleMode(QWidget *parent = 0, int mode = 1);
    ~SingleMode();
    void setMode(int mode);
    void pause();
public slots:
    void mute();
    void unmute();
    void saveGame();
    void loadGame();
private slots:
    void startProgress();
protected:
    void paintEvent(QPaintEvent *event) override;
    void loadingStatus(int);
    void keyPressEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void returnToHomepage();
private:
    int currrentLighted = 0;
    int mode;
    int lightTime = 15;
    int carriedCode = 99;
    int flashCount = 5;
    Boss *boss = nullptr;
    int marks = 0;
    QGraphicsScene *scene = nullptr;
    QPixmap *background = nullptr;
    QPixmap *smallChara = nullptr;
    Player player;
    charaSelection *cSlection = nullptr;
    returnButton *btnBack = nullptr;
    Ui::SingleMode *ui;
    int step;
    QGraphicsPixmapItem *leftBar = nullptr, *rightBar = nullptr, *upBar = nullptr, *downBar = nullptr;
    QGraphicsPixmapItem *areaForDamage = nullptr, *areaForThirsty = nullptr, *areaForHealing = nullptr;
    QTimer aDamTimer, aHealTimer, aThirTimer, alreadyThirstyTimer, drinkingTimer, losingWaterTimer;
    QTimer *PaceSlowTimer = nullptr, *MPStrictTimer = nullptr;
    QTimer *PaceFastTimer = nullptr;
    QTimer *FlashTimer = nullptr;
    Blocks* blocks[12];
    bool faster = false;
    bool paused = true;
    bool MPStricted = false;
    bool firstTime = true;
    bool failed = false;
    void addBlocks();
    void addBoss();
    void addBossSkills();
    void checkHp();
    void addBar();
    void setClocks();
    void setRules();
    void setArea();
    void showFail();
    void showVictory();
    void showShaken();
    void showTransparent();
    int decreaseHp(int value);
    int decreaseMp(int value);
    int carriedState = 99;
    bool volumeIsOn = true;
    bool ableToFlash = false;
    bool ableToDrink = true;
    QTimer *fighting1Timer = nullptr;
    QTimer *fighting2Timer = nullptr;
    QSound *drinking = new QSound(":/soundEffects/music/DrinkingWater.wav", this);
    QSound *fighting1 = new QSound(":/soundEffects/music/DatingStart.wav", this);
    QSound *fighting2 = new QSound(":/soundEffects/music/Bonetrousle.wav", this);
    QSound *fighting3 = new QSound(":/soundEffects/music/Fighting.wav", this);
    QSound *playerBeenHurt = new QSound(":/soundEffects/music/PlayerBeenHurt.wav", this);
    QSound *showFailing = new QSound(":/soundEffects/music/ShowFail.wav",this);
    QSound *Walking = new QSound(":/soundEffects/music/Walking.wav", this);
    QSound *showTheVictory = new QSound(":/soundEffects/music/ShowVictory.wav", this);
    QSound *takeItem = new QSound(":/soundEffects/music/TakeItem.wav", this);
    QTimer *lightTimer = nullptr;

    void judgeEliminate();
    void judgeCollide(bool closeToWater);
    void connectChara();
    void lightBlock();
    bool judgeLighintNeed();
    void shuffle();
    void hint();
    void speedBster();
    void HPBster();
    void MPBster();
    void high();
    void hit();
    void specialHit();
    void flash();
    void addTime();
};

#endif // SINGLEMODE_H
