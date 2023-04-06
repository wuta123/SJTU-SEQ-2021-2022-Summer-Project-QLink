#include "charaselection.h"
//是服务于game的一个人物选择小类，包括了人物贴图、以及鼠标在不同人物上方悬停时候的人物状态。
//其中按钮用了蒙版，以人物的形象显示
charaSelection::charaSelection(QWidget *parent):QWidget(parent)
{
    QPixmap charaSelectionPix;
    charaSelectionPix.load(":/image/image/manSelection.jpg",0, Qt::AvoidDither|Qt::ThresholdDither|Qt::ThresholdAlphaDither);
    resize(charaSelectionPix.size());
    setMask(QBitmap(charaSelectionPix.mask()));
    loadingChara();
}

void charaSelection::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/image/image/manSelection.jpg"));
}

void charaSelection::loadingChara()
{

    chara1.setStyleSheet("QPushButton{border-image: url(:/chara/character/cloudTheWarrior_01.png);}"
                        "QPushButton:hover{border-image: url(:/chara/character/cloudTheWarrior_01_Hover.png);}"
                        "QPushButton:pressed{border-image: url(:/chara/character/cloudTheWarrior_01_Hover.png);}");
    chara2.setStyleSheet("QPushButton{border-image: url(:/chara/character/darkWarrior_01.png);}"
                        "QPushButton:hover{border-image: url(:/chara/character/darkWarrior_01_Hover.png);}"
                        "QPushButton:pressed{border-image: url(:/chara/character/darkWarrior_01_Hover.png);}");
    chara3.setStyleSheet("QPushButton{border-image: url(:/chara/character/redHeadBandKnight_01.png);}"
                        "QPushButton:hover{border-image: url(:/chara/character/redHeadBandKnight_01_Hover.png);}"
                        "QPushButton:pressed{border-image: url(:/chara/character/redHeadBandKnight_01_Hover.png);}");
    chara4.setStyleSheet("QPushButton{border-image: url(:/chara/character/thief_01.png);}"
                        "QPushButton:hover{border-image: url(:/chara/character/thief_01_Hover.png);}"
                        "QPushButton:pressed{border-image: url(:/chara/character/thief_01_Hover.png);}");
    chara1.setParent(this);
    chara2.setParent(this);
    chara3.setParent(this);
    chara4.setParent(this);

    chara1.resize(166,250);
    chara2.resize(166,250);
    chara3.resize(166,250);
    chara4.resize(166,250);

    chara1.move(50,20);
    chara2.move(504,20);
    chara3.move(958,20);
    chara4.move(1412,20);

    connect(&chara1, &QPushButton::clicked, this, &charaSelection::chara1Selected);
    connect(&chara1, &QPushButton::clicked, this, &charaSelection::close);
    connect(&chara2, &QPushButton::clicked, this, &charaSelection::chara2Selected);
    connect(&chara2, &QPushButton::clicked, this, &charaSelection::close);
    connect(&chara3, &QPushButton::clicked, this, &charaSelection::chara3Selected);
    connect(&chara3, &QPushButton::clicked, this, &charaSelection::close);
    connect(&chara4, &QPushButton::clicked, this, &charaSelection::chara4Selected);
    connect(&chara4, &QPushButton::clicked, this, &charaSelection::close);
}
