#pragma once
#ifndef PLAYERT_H
#define PLAYERT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsItemAnimation>
class PlayerT : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    int width;
    int height;
    int step;
    PlayerT();

    void setCurrentDirection(int num);
    void timerEvent(QTimerEvent *event) override;
    QRectF boundingRect() const override;;
    QPainterPath shape() const override;
public slots:
    void changeShape();
    void backToNormal();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QRectF normalShape;
    QRectF leftShape, rightShape, upShape, downShape;
    QRectF currentShape;
    void loadingChara();
    QTimer *animationTimer;
    QGraphicsItemAnimation *anim;
    QPixmap image;
    QPixmap pix_right[4];
    QPixmap pix_left[4];
    QPixmap pix_up[4];
    QPixmap pix_down[4];
    int currentDirection = 4;
    int currentFrame = 0;
    int startX;
    int startY;
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // PLAYERT_H
