#include "block.h"

Blocks::Blocks(int mode)
{
    /*道具的初始化，并且定义几个需要使用的时钟功能*/
    reborn();
    this->mode = mode;
    count = 15;
    livingTimer = new QTimer;
    livingTimer->setInterval(1000);
    rebornTimer = new QTimer;
    flashTimer = new QTimer;
    flashTimer -> setInterval(500);
    setValues();
    connect(livingTimer, &QTimer::timeout, this, &Blocks::checkLife);
    connect(rebornTimer, &QTimer::timeout, this, [=](){
        reborned = false;
        this -> show();
        rebornTimer->stop();
        livingTimer->start();
    });
    connect(flashTimer, &QTimer::timeout, [=](){
        if(!flash){
            this->hide();
        }else{
            this->show();
        }
        flash = !flash;
    });
}

Blocks::Blocks(Blocks &block){
    /*道具的复制构造函数*/
    carried = false;
    lighted = false;
    X = 275+rand() % 1030;
    Y = 95+rand() % 705;
    setPos(X, Y);
    state = block.state;
    mode = block.mode;
    loadingBlock(int(state));
    update();
    count = 15;
    livingTimer = new QTimer;
    livingTimer->setInterval(1000);
    rebornTimer = new QTimer;
    flashTimer = new QTimer;
    flashTimer -> setInterval(500);
    setValues();
    connect(livingTimer, &QTimer::timeout, this, &Blocks::checkLife);
    connect(rebornTimer, &QTimer::timeout, this, [=](){
        reborned = false;
        this -> show();
        rebornTimer->stop();
        livingTimer->start();
    });
    connect(flashTimer, &QTimer::timeout, [=](){
        if(!flash){
            this->hide();
        }else{
            this->show();
        }
        flash = !flash;
    });
}
void Blocks::randomState(){
    /*给新生成的道具随机赋予一种状态，概率有区别*/
    int a = rand() % 50 + 1;
    switch(a){
        case 6:case 3:case 12:case 16:case 22:
        case 50:case 45:case 44:case 8:case 38:
        case 49:case 46:case 40:case 37:case 34:
        case 47:case 42:case 41:case 36:case 30:
        state = NormalItem;
        break;
    case 10:case 2:case 17:case 27:
        state = MpBooster;
        break;
    case 7:case 5:case 18:case 26:
        state = SpeedBooster;
        break;
    case 9:case 19:case 35:case 24:
        state = Hint;
        break;
    case 4:case 13:
        state = HpBooster;
        break;
    case 1:case 15:case 20:case 43:
        state = SpecialHit;
        break;
    case 21: case 11:
        state = SuperItem;
        break;
    case 25: case 48:
        state = Shuffle;
        break;
    case 28: case 31:case 23:case 14:
        state = Flash;
        break;
    case 39: case 32: case 29:case 33:
        state = AddTime;
        break;
    default:
        state = NormalItem;
        break;
    }
}

void Blocks::shadow()
{
    /*从高亮状态中脱离*/
    lighted = false;
    update();
}

void Blocks::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*绘制方块*/
    if(!lighted)
        painter -> drawPixmap(boundingRect().topLeft(), pix);
    else
        painter -> drawPixmap(boundingRect().topLeft(), lightedPix);
}

QRectF Blocks::boundingRect() const
{
    return QRectF(0,0,width,height);
}

bool Blocks::returnCount(){
    /*在高亮方块时会标记能够在场地上存在一段时间的方块*/
     bool a = count >= 4 && this->isVisible();
     return a;
}

bool Blocks::merging()
{
    /*标记这个方块正在被融合，以避免一些道具对它产生不必要的作用*/
    return merge;
}

void Blocks::setValues()
{
    /*设定场中部分道具的数值*/
    HpBster = (mode == 1) ? 35:25;
    MpBster = (mode == 1) ? 45:30;
    SpeedBster = (mode == 1) ? 5:4;
}

void Blocks::loadingBlock(int state)
{
    /*根据道具的state来加载道具的贴图*/
    switch(state){
    case 0:
        pix.load(":/item/Items/NormalItem.png");
        lightedPix.load(":/item/Items/NormalItem_lighted.png");
        break;
    case 1:
        pix.load(":/item/Items/SpecialHit.png");
        lightedPix.load(":/item/Items/SpecialHit_lighted.png");
        break;
    case 2:
        pix.load(":/item/Items/HpBster.png");
        lightedPix.load(":/item/Items/HpBster_lighted.png");
        break;
    case 3:
        pix.load(":/item/Items/MpBster.png");
        lightedPix.load(":/item/Items/MpBster_lighted.png");
        break;
    case 4:
        pix.load(":/item/Items/SpeedBster.png");
        lightedPix.load(":/item/Items/SpeedBster_lighted.png");
        break;
    case 5:
        pix.load(":/item/Items/Hint.png");
        lightedPix.load(":/item/Items/Hint_lighted.png");
        break;
    case 6:
        pix.load(":/item/Items/SuperItem.png");
        lightedPix.load(":/item/Items/SuperItem_lighted.png");
        break;
    case 7:
        pix.load(":/item/Items/Shuffle.png");
        lightedPix.load(":/item/Items/Shuffle_lighted.png");
        break;
    case 8:
        pix.load(":/item/Items/Flash.png");
        lightedPix.load(":/item/Items/Flash_beenLighted.png");
        break;
    case 9:
        pix.load(":/item/Items/FiveMoreSeconds.png");
        lightedPix.load(":/item/Items/FiveMoreSeconds_beenLighted.png");
        break;
    default:
        pix.load(":/item/Items/NormalItem.png");
        lightedPix.load(":/item/Items/NormalItem_lighted.png");
    }
    width = lightedPix.width();
    height = lightedPix.height();
}

void Blocks::startFlashing()
{
    /*在道具快要消失的时候开始高闪*/
    flashTimer -> start();

}

void Blocks::mute()
{
    /*静音*/
    volume = false;
}

void Blocks::loadState(int state){
    /*配合loadGame使用的，从文件中加载出道具状态*/
    switch(state){
    case 0:
        this->state = NormalItem;
        break;
    case 1:
        this->state = SpecialHit;
        break;
    case 2:
        this->state = HpBooster;
        break;
    case 3:
        this->state = MpBooster;
        break;
    case 4:
        this->state = SpeedBooster;
        break;
    case 5:
        this->state = Hint;
        break;
    case 6:
        this->state = SuperItem;
        break;
    case 7:
        this->state = Shuffle;
        break;
    case 8:
        this->state = Flash;
        break;
    case 9:
        this->state = AddTime;
    }
    loadingBlock(state);
}

void Blocks::unmute(){
    /*开声音*/
    volume = true;
}

bool Blocks::checkLighted()
{
    return lighted;
}

void Blocks::beenReversed()
{
    /*reversed状态表示这枚道具是否收到boss技能的影响,在受影响和影响消除的时候均会调用该函数*/
    Reversed = !Reversed;
}

bool Blocks::checkReverse(){
    return Reversed;
}

void Blocks::checkLife()
{
    /*决定道具寿命，以及在其寿命将尽时开始高闪*/
    if(count == 3) startFlashing();
    if(count >= 0)
        count--;
    else{
        livingTimer->stop();
        count = 15;
        disappear();
    }
}

int Blocks::checkState()
{
    return (int)state;
}

void Blocks::disappear()
{
    /*在道具被融合、寿命结束、融合失败（被丢弃}时调用*/
    this->hide();
    pause();
    reborn();
    rebornTimer -> setInterval(rand() % 6000);
    rebornTimer -> start();
    reborned = true;
}

QPixmap Blocks::getPix()
{
    return pix;
}

void Blocks::startLife()
{
    /*道具重生结束*/
    if(!reborned)livingTimer->start();
    if(!reborned && count <= 3)flashTimer->start();
    if(reborned) rebornTimer->start();
}

void Blocks::pause(){
    /*在暂停的时候停止道具的时针*/
    livingTimer->stop();
    flashTimer->stop();
    rebornTimer->stop();
}

void Blocks::reborn()
{
    /*道具的重生函数*/
    randomState();
    loadingBlock(int(state));
    carried = false;
    flash = false;
    lighted = false;
    Reversed = false;
    merge = false;
    if(state == NormalItem){
        X = 275+rand() % 1030;
        Y = 95+rand() % 705;
    }else{
        int b = rand() % 4 + 1;
        switch (b) {
            case 1:
                X = 275+rand() % 1030;
                Y = 95+rand() % 705;
                break;
            default:
                X = 275+rand() % 1030;
                Y = 95+rand() % 340;
            break;
        }//用意是让特殊道具更可能生成在地图上侧的两个危险区域
    }
    setPos(X,Y);
    update();
}

void Blocks::beenCarried(){
    /*标记道具的被拾取状态*/
    pause();
    this->hide();
    carried = true;
}

void Blocks::beenLighted()
{
    /*高亮道具*/
    lighted = true;
    update();
}

bool Blocks::checkCarried()
{
    return carried;
}

QPointF Blocks::currentPos(){
    return QPointF(X,Y);
}

void Blocks::beenMerged(){
    /*道具被融合后发出不同提示声,发出不同信号给主游戏窗口*/
    int a = state;
    switch (a) {
        case 4://speedBuster
            speedBster->setLoops(1);
            speedBster->play();
            break;
        case 5://Hint
            hint->setLoops(1);
            hint->play();
            break;
        case 6://SuperItem
            high->setLoops(1);
            high->play();
            break;
        case 7://shuffle
            hint->setLoops(1);
            hint->play();
            break;
        case 8://flash
            hint->setLoops(1);
            hint->play();
        case 9://+5s
            hint->setLoops(1);
            hint->play();
        default:
            break;
    }
    emit causeEffects((int)state);
    merge = true;
}
