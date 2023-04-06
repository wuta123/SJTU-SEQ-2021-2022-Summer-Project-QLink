#pragma once
#ifndef Blocks_H
#define Blocks_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QPointF>
#include <QSound>
enum BlocksStat
{
    NormalItem,//对Boss造成正常伤害，暂定为15
    SpecialHit,//对Boss造成3倍伤害，即45点伤害
    HpBooster,//对Boss造成10点伤害，增加血量，在mode为1时增加15，在mode为2或者mode为3时增加10
    MpBooster,//对Boss造成10点伤害，增加MP值，在mode为1时增加20，在mode为2或者3时增加10
    SpeedBooster,//对Boss造成正常伤害，增加速度，在mode为1时速度增加3，在mode为2或者3时增加2
    Hint,//对Boss造成正常伤害，并快速找到两个可以配对的方块，将其高亮显示
    SuperItem,//造成除hint以外以上的所有效果
    Shuffle,//洗牌
    Flash,
    AddTime
};


class Blocks : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Blocks(int mode);
    Blocks(Blocks &block);
    int HpBsterValue() const{
        return HpBster;
    }
    int MpBsterValue() const{
        return MpBster;
    }
    int SpeedBsterValue() const{
        return SpeedBster;
    }
    bool checkReverse();
    bool checkLighted();
    bool checkCarried();
    void checkLife();
    int checkState();
    void disappear();
    QPixmap getPix();
    QPointF currentPos();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
signals:
    void causeEffects(int value);
private:
    QTimer *rebornTimer;
    int X, Y;
    bool merge = false;
    QTimer *flashTimer;
    int mode;
    int width;
    int height;
    int HpBster, MpBster, SpeedBster;
    void setValues();
    bool flash = false;
    bool lighted = false;
    bool Reversed = false;
    bool carried = false;
    bool reborned = false;
    bool volume = true;
    BlocksStat state;//这是哪一种方块
    QPixmap pix;
    QPixmap lightedPix;
    QTimer *livingTimer;
    int count;//物品在地图中能够存在的时间
    void loadingBlock(int state);
    void startFlashing();
    QSound *speedBster = new QSound(":/soundEffects/music/SpeedBster.wav", this);
    QSound *high = new QSound(":/soundEffects/music/high.wav", this);
    QSound *hint = new QSound(":/soundEffects/music/Hint.wav", this);
public slots:
    void mute();
    void unmute();
    void beenReversed();
    void beenCarried();//被携带，从地图上消失
    void beenLighted();
    void startLife();
    void pause();
    void reborn();
    void beenMerged();//两个相同的方块合二为一了
    void randomState();
    void shadow();
    bool returnCount();
    bool merging();
    void loadState(int state);
};

#endif // Blocks_H
