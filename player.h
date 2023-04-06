#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsItemAnimation>
#include <QTimer>
#include <QPainterPath>
class Player : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    int charaType = 0;
    int width;
    int height;
    int step;
    Player(QObject *parent = nullptr);
    ~Player();
    void setCurrentDirection(int num);
    //void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *event) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
private:
    QTimer *animationTimer;
    QGraphicsItemAnimation *anim;
    QImage image;
    QPixmap customChara;
    QPixmap pix_right[4];
    QPixmap pix_left[4];
    QPixmap pix_up[4];
    QPixmap pix_down[4];
    int currentDirection = 4;// 1 = right, 2 = left, 3 = up, 4 = down;
    int currentFrame = 0;
    int startX;
    int startY;
    int times = 2;//times=2对应的是普通状态，times = 1对应急速状态, times = 3对应缓慢状态



public slots:
    void loadingChara(int number = 0);
public:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PLAYER_H
