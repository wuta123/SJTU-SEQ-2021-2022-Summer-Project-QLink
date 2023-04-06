#pragma once
#ifndef BLOCKST_H
#define BLOCKST_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QPointF>
#include <QPainterPath>
#include <QMouseEvent>
enum BlocksState
{
    NormalItemT,
    SpecialHitT,
    HpBoosterT,
    MpBoosterT,
    SpeedBoosterT,
    HintT,
    SuperItemT,
    ShuffleT,//洗牌
    FlashT,//闪现
    AddTimeT//加时
};

class BlocksT : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:

    BlocksT(int x, int y, int code);
    BlocksT(BlocksT &block, int x, int y, int code);
    bool checkCarried();
    int checkState();
    bool checkLighted();
    int getX() const{return X;}
    int getY() const{return Y;}
    bool paused = false;
signals:
    void causeEffects(int value);
    void beenClicked(int value);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    const int width = 45;
    const int height = 45;
    int X,Y;//表示这个方块位于方块堆中的第几行第几列
    bool carried = false;
    bool lighted = false;
    int code;
    QPixmap pix;
    QPixmap lightedPix;
    void loadingBlock(int state);
    BlocksState state;
public slots:
    void beenLighted();//收到玩家的信号而被激活
    void beenMerged();//融合
    void shadow();//在没能被成功配对的时候解除高亮状态
    void randomState();
    void loadState(int state);//在读取游戏时，加载这个物品的状态
    void beenCarried();
    void setX(int x){X = x;}
    void setY(int y){Y = y;}
};

#endif // BLOCKST_H
