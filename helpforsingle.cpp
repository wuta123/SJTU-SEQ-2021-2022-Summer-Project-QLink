#include "helpforsingle.h"
#include <QMainWindow>
#include "returnbutton.h"
#include <QPainter>
#include <QFile>
#include <QFileInfo>
/*单人模式的帮助说明页面，以及通过不同的button选择不同的游戏难度，以及一个returnButton可以回到主页面*/
HelpForSingle::HelpForSingle(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("帮助-单人模式"));
    setFont(QFont("Microsoft Yahei", 9));
    //窗口标题
    setWindowIcon(QIcon(":/icon/1x1/darkzaitzev/chameleon-glyph.svg"));
    rButton.setParent(this);
    rButton.resize(100,100);
    rButton.move(1450,780);
    connect(&rButton,&QPushButton::clicked,[=](){
        this->hide();
        this->emit rSignal();
    });

    btn = new QPushButton(this);
    btn->setText(QStringLiteral("简单模式"));
    btn->resize(100,100);
    btn->move(450,750);
    btn2 = new QPushButton(this);
    btn2->setText(QStringLiteral("普通模式"));
    btn2->resize(100,100);
    btn2->move(650,750);
    btn3 = new QPushButton(this);
    btn3->setText(QStringLiteral("困难模式"));
    btn3->resize(100,100);
    btn3->move(850,750);
    btn4 = new QPushButton(this);
    btn4->setText(QStringLiteral("加载游戏"));
    btn4->resize(100,100);
    btn4->move(1050,750);
    btn5 = new QPushButton(this);
    btn5->setText(u8"答辩模式");
    btn5->resize(100,100);
    btn5->move(1450,660);
    judgeButtonAvaliablity();
    connectButtonFunction();
}

void HelpForSingle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/image/the_BackGroundForHelp.png");
    painter.drawPixmap(this->rect(), pix);
}

void HelpForSingle::connectButtonFunction(){
    connect(btn,&QPushButton::clicked,this,[=](){
        sGame = new SingleMode(nullptr,1);
        connect(sGame, &SingleMode::returnToHomepage, this, [=](){
            delete sGame;
            emit rSignal();
        });
        sGame -> show();
        this -> hide();
    });
    connect(btn2,&QPushButton::clicked,this,[=](){
        sGame = new SingleMode(nullptr,2);
        connect(sGame, &SingleMode::returnToHomepage, this, [=](){
            delete sGame;
            emit rSignal();
        });
        sGame -> show();
        this -> hide();
    });
    connect(btn3,&QPushButton::clicked,this,[=](){
        sGame = new SingleMode(nullptr,3);
        connect(sGame, &SingleMode::returnToHomepage,this, [=](){
            delete sGame;
            emit rSignal();
        });
        sGame -> show();
        this -> hide();
    });
    connect(btn4,&QPushButton::clicked,this,[=](){
        sGame = new SingleMode(nullptr,4);
        connect(sGame, &SingleMode::returnToHomepage, this, [=](){
            delete sGame;
            emit rSignal();
        });
        sGame -> show();
        this -> hide();
    });
    connect(btn5, &QPushButton::clicked, this, [=](){
        mapEdit = new MapEdit;
        mapEdit->show();
        connect(mapEdit, &MapEdit::startGame, this,[=](int x, int y, int firstTime){
            sGameT = new SingleModeT(x, y, firstTime);
            sGameT->show();
            connect(sGameT, &SingleModeT::returnToHomepage, this, [=](){
                delete sGameT;
                emit rSignal();
            });
            this->hide();
        });
    });
}

void HelpForSingle::judgeButtonAvaliablity(){
    /*通过读取存档文件来设定按钮的enabled状态*/
    QFileInfo file("C:\\temp\\saver.txt");
    if(file.isFile()){
        reader = new QFile("C:\\temp\\saver.txt");
        if(reader->open(QIODevice::ReadOnly)){
        QString a = reader->readAll();
        if(a[0] == '1'){
            btn->setEnabled(true);
            btn2->setEnabled(true);
            btn3->setEnabled(true);
        }
        if(a[0] == '2'){
            btn->setEnabled(true);
            btn2->setEnabled(true);
            btn3->setEnabled(true);
        }
        if(a[0] == '3'){
            btn->setEnabled(true);
            btn2->setEnabled(true);
            btn3->setEnabled(true);
        }
        reader->close();
        }
    }
    else{
        reader = new QFile("C:\\temp\\saver.txt");
        if(reader->open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QString a = QString::number(1);
        reader->write(a.toUtf8());
        btn->setEnabled(true);
        btn2->setEnabled(true);
        btn3->setEnabled(true);
        reader->close();
        }
    }

    btn4->setEnabled(false);

    QFileInfo file2("C:\\temp\\isSaved.txt");
    if(file2.isFile()){
        reader = new QFile("C:\\temp\\isSaved.txt");
        if(reader->open(QIODevice::ReadOnly)){
            QString a = reader->readAll();
            if(a.toInt() == 1 )
                btn4->setEnabled(true);
            reader->close();
        }
    }
}
