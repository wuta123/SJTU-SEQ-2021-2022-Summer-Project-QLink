#include "blockst.h"

BlocksT::BlocksT(int x, int y, int code)
{/*x,y为该方块在方块堆内部的位置*/
    this->X = x;
    this->Y = y;
    this->code = code;
    randomState();
    loadingBlock(int(state));
    update();
}

BlocksT::BlocksT(BlocksT &block, int x, int y, int code)
{
    this->state = block.state;
    this->X = x;
    this->Y = y;
    this->code = code;
    loadingBlock(int(state));
    update();
}

bool BlocksT::checkCarried()
{
    return carried;
}

int BlocksT::checkState()
{
    return int(state);
}

bool BlocksT::checkLighted()
{
    return lighted;
}

void BlocksT::randomState()
{
    /*给新生成的道具随机赋予一种状态，概率有些许区别*/
    int a = rand() % 20 + 1;
    switch(a){
    case 1: case 20:
        state = NormalItemT;
        break;
    case 18: case 4:
        state = MpBoosterT;
        break;
    case 5: case 15:
        state = SpeedBoosterT;
        break;
    case 7: case 13:
        state = HintT;
        break;
    case 9: case 10:
        state = HpBoosterT;
        break;
    case 11: case 14:
        state = SpecialHitT;
        break;
    case 8: case 12:
        state = SuperItemT;
        break;
    case 6: case 16:
        state = ShuffleT;
        break;
    case 17: case 3:
        state = FlashT;
        break;
    case 19: case 2:
        state = AddTimeT;
        break;
    default:
        state = NormalItemT;
        break;
    }
}

void BlocksT::shadow()
{
    /*道具消失*/
    lighted = false;
    update();
}

void BlocksT::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!lighted)
        painter -> drawPixmap(boundingRect().topLeft(), pix);
    else
        painter -> drawPixmap(boundingRect().topLeft(), lightedPix);
}

QRectF BlocksT::boundingRect() const
{
    return QRectF(0,0,width,height);
}

QPainterPath BlocksT::shape() const
{
    //通过重载shape函数来给方块设定更为精确的碰撞箱
    QPainterPath path;
    path.addRect(0,0,width-2,height-2);
    return path;
}

void BlocksT::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!paused && event->button() == Qt::LeftButton)
        emit beenClicked(code);
}

void BlocksT::beenMerged(){
    emit causeEffects(int(state));
}

void BlocksT::beenLighted(){
    lighted = true;
    update();
}

void BlocksT::beenCarried(){
    carried = true;
}

void BlocksT::loadingBlock(int state)
{
    /*根据道具的state来加载道具的贴图*/
    switch(state){
    case 0:
        pix.load(":/test/test/NormalItem.png");
        lightedPix.load(":/test/test/NormalItem_lighted.png");
        break;
    case 1:
        pix.load(":/test/test/SpecialHit.png");
        lightedPix.load(":/test/test/SpecialHit_lighted.png");
        break;
    case 2:
        pix.load(":/test/test/HpBster.png");
        lightedPix.load(":/test/test/HpBster_lighted.png");
        break;
    case 3:
        pix.load(":/test/test/MpBster.png");
        lightedPix.load(":/test/test/MpBster_lighted.png");
        break;
    case 4:
        pix.load(":/test/test/SpeedBster.png");
        lightedPix.load(":/test/test/SpeedBster_lighted.png");
        break;
    case 5:
        pix.load(":/test/test/Hint.png");
        lightedPix.load(":/test/test/Hint_lighted.png");
        break;
    case 6:
        pix.load(":/test/test/SuperItem.png");
        lightedPix.load(":/test/test/SuperItem_lighted.png");
        break;
    case 7:
        pix.load(":/test/test/Shuffle.png");
        lightedPix.load(":/test/test/Shuffle_lighted.png");
        break;
    case 8:
        pix.load(":/test/test/Flash.png");
        lightedPix.load(":/test/test/Flash_beenLighted.png");
        break;
    case 9:
        pix.load(":/test/test/FiveMoreSeconds.png");
        lightedPix.load(":/test/test/FiveMoreSeconds_beenLighted.png");
        break;
    default:
        pix.load(":/test/test/NormalItem.png");
        lightedPix.load(":/test/test/NormalItem_lighted.png");
    }
    update();
}

void BlocksT::loadState(int state){
    /*配合loadGame使用的，从文件中加载出道具状态*/
    switch(state){
    case 0:
        this->state = NormalItemT;
        break;
    case 1:
        this->state = SpecialHitT;
        break;
    case 2:
        this->state = HpBoosterT;
        break;
    case 3:
        this->state = MpBoosterT;
        break;
    case 4:
        this->state = SpeedBoosterT;
        break;
    case 5:
        this->state = HintT;
        break;
    case 6:
        this->state = SuperItemT;
        break;
    case 7:
        this->state = ShuffleT;
        break;
    case 8:
        this->state = FlashT;
        break;
    case 9:
        this->state = AddTimeT;
    }
    loadingBlock(state);
}

