#include "player.h"
#include <QTimeLine>
#include <QPainter>
#include <QGraphicsItemAnimation>
#include <QPixmap>
Player::Player(QObject *parent):QObject(parent)
{//初始化一些特性以及走路的速度
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    loadingChara(0);
    step = 6;
}

Player::~Player()
{

}

void Player::setCurrentDirection(int num)
{
    currentDirection = num;//以数字表示方向
}

void Player::mousePressEvent(QGraphicsSceneMouseEvent *event){
//空函数，确保鼠标无法对小人进行操作
}

void Player::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
//空函数，确保鼠标操作对小人无效
}

void Player::timerEvent(QTimerEvent *event)
{//在小人移动之后及时更新并重新绘制小人
    update();
}

QRectF Player::boundingRect() const
{
    return QRectF(0,0, width, height);

}

QPainterPath Player::shape() const
{
    //通过重载shape函数来给小人设定更为精确的碰撞箱
    QPainterPath path;
    path.addRect(0,30,32,18);
    return path;
}

void Player::loadingChara(int number)
{
    //加载人物贴图，将人物移动的动画分到四个不同的组内，方便调用
    charaType = number;
    switch (number) {
    case 0:
        customChara.load(":/chara/character/cloudTheWarrior.png");
        image.load(":/chara/character/cloudTheWarrior.png");
        break;
    case 1:
        customChara.load(":/chara/character/darkWarrior.png");
        image.load(":/chara/character/darkWarrior.png");
        break;
    case 2:
        customChara.load(":/chara/character/redHeadBandKnight.png");
        image.load(":/chara/character/redHeadBandKnight.png");
        break;
    case 3:
        customChara.load(":/chara/character/thief.png");
        image.load(":/chara/character/thief.png");
        break;
    default:
        break;
    }

    width = customChara.width()/4;
    height = customChara.height()/4;

    pix_right[0] = customChara.copy(0,96,32,48);
    pix_right[1] = customChara.copy(32,96,32,48);
    pix_right[2] = customChara.copy(64,96,32,48);
    pix_right[3] = customChara.copy(96,96,32,48);

    pix_left[0] = customChara.copy(0,48,32,48);
    pix_left[1] = customChara.copy(32,48,32,48);
    pix_left[2] = customChara.copy(64,48,32,48);
    pix_left[3] = customChara.copy(96,48,32,48);

    pix_down[0] = customChara.copy(0,0,32,48);
    pix_down[1] = customChara.copy(32,0,32,48);
    pix_down[2] = customChara.copy(64,0,32,48);
    pix_down[3] = customChara.copy(96,0,32,48);

    pix_up[0] = customChara.copy(0,144,32,48);
    pix_up[1] = customChara.copy(32,144,32,48);
    pix_up[2] = customChara.copy(64,144,32,48);
    pix_up[3] = customChara.copy(96,144,32,48);

}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //移动时同步调用用来绘制小人的动态
    switch (currentDirection) {
        case 1:
            painter->drawPixmap(boundingRect().topLeft(), pix_right[currentFrame/times]);
            if(currentFrame < 3*times) currentFrame++;
            else currentFrame = 0;
            break;
        case 2:
            painter->drawPixmap(boundingRect().topLeft(), pix_left[currentFrame/times]);
            if(currentFrame < 3*times) currentFrame++;
            else currentFrame = 0;
            break;
        case 3:
            painter->drawPixmap(boundingRect().topLeft(), pix_up[currentFrame/times]);
            if(currentFrame < 3*times) currentFrame++;
            else currentFrame = 0;
            break;
        case 4:
            painter->drawPixmap(boundingRect().topLeft(), pix_down[currentFrame/times]);
            if(currentFrame < 3*times) currentFrame++;
            else currentFrame = 0;
            break;
        default:
            break;
    }
}

