#include "playert.h"
#include <QTimeLine>
#include <QPainter>
#include <QPixmap>

PlayerT::PlayerT()
{

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);

    step = 6;
    loadingChara();
}

void PlayerT::setCurrentDirection(int num){
    currentDirection = num;
}

void PlayerT::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //空函数，确保鼠标无法对小人进行操作
}

void PlayerT::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    //空函数，确保鼠标操作对小人无效
}

void PlayerT::timerEvent(QTimerEvent *event)
{   //在小人移动之后及时更新并重新绘制小人
    update();
}

QRectF PlayerT::boundingRect() const
{
    return QRectF(0,0,width,height);

}

QPainterPath PlayerT::shape() const
{
    //通过重载shape函数来给小人设定更为精确的碰撞箱
    QPainterPath path;
    path.addRect(normalShape);
    return path;
}

void PlayerT::changeShape()
{
    switch (currentDirection) {
        case 1:
            currentShape = rightShape;
            break;
        case 2:
            currentShape = leftShape;
            break;
        case 3:
            currentShape = upShape;
            break;
        case 4:
            currentShape = downShape;
            break;
    }
    shape();
}

void PlayerT::backToNormal(){
    currentShape = normalShape;
    shape();
}

void PlayerT::loadingChara(){
    image.load(":/chara/character/redHeadBandKnight.png");

    width = image.width()/4;
    height = image.height()/4;

    normalShape = currentShape = QRectF(0,10,32,30);
    leftShape = QRectF(-40, height/2-1, width+45, 2);
    rightShape = QRectF(5, height/2-1, width+45,2);
    upShape = QRectF(width/2-2, -20, 4, height+25);
    downShape = QRectF(width/2-2, 5, 4, height+25);

    pix_right[0] = image.copy(0,96,32,48);
    pix_right[1] = image.copy(32,96,32,48);
    pix_right[2] = image.copy(64,96,32,48);
    pix_right[3] = image.copy(96,96,32,48);

    pix_left[0] = image.copy(0,48,32,48);
    pix_left[1] = image.copy(32,48,32,48);
    pix_left[2] = image.copy(64,48,32,48);
    pix_left[3] = image.copy(96,48,32,48);

    pix_down[0] = image.copy(0,0,32,48);
    pix_down[1] = image.copy(32,0,32,48);
    pix_down[2] = image.copy(64,0,32,48);
    pix_down[3] = image.copy(96,0,32,48);

    pix_up[0] = image.copy(0,144,32,48);
    pix_up[1] = image.copy(32,144,32,48);
    pix_up[2] = image.copy(64,144,32,48);
    pix_up[3] = image.copy(96,144,32,48);
}

void PlayerT::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    //移动时同步调用用来绘制小人的动态
    switch (currentDirection) {
        case 1:
            painter->drawPixmap(boundingRect().topLeft(), pix_right[currentFrame]);
            if(currentFrame < 3) currentFrame++;
            else currentFrame = 0;
            break;
        case 2:
            painter->drawPixmap(boundingRect().topLeft(), pix_left[currentFrame]);
            if(currentFrame < 3) currentFrame++;
            else currentFrame = 0;
            break;
        case 3:
            painter->drawPixmap(boundingRect().topLeft(), pix_up[currentFrame]);
            if(currentFrame < 3) currentFrame++;
            else currentFrame = 0;
            break;
        case 4:
            painter->drawPixmap(boundingRect().topLeft(), pix_down[currentFrame]);
            if(currentFrame < 3) currentFrame++;
            else currentFrame = 0;
            break;
        default:
            break;
    }
}
