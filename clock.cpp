#include "clock.h"
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
/*clock类用于记录游戏的剩余时间、以及在被点击时调出暂停窗口*/
Clock::Clock(QWidget *parent)
    : QLCDNumber(parent)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::red);
    setPalette(p);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.5);
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    showTime();
    resize(250,60);
    setSec(sec);

}

Clock::~Clock()
{

}

int Clock::remainTime(){
    return sec;
}

void Clock::setRemainTime(int value){
    sec = value;
    showTime();
}

void Clock::mousePressEvent(QMouseEvent *event)
{
    if(event -> buttons() & Qt::LeftButton){
        pause();
        emit isPaused();
    }
}

void Clock::showTime(){
    if(sec > 0)
        sec--;
    else{
        emit timesUp();
        return;
    }
    setSec(sec);
}

void Clock::pause()
{
    timer->stop();
}

void Clock::restart(){
    if(firstTime)
        firstTime = 0;
    else
        showTime();
    timer->start();
}

void Clock::setSec(int value)
{
    sec = value;
    int m = value/60;
    value %= 60;
    QString str = QString::number(m)+":"+QString::number(value);
    display(str);
}
