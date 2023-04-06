#pragma once
#ifndef DIGICLOCK_H
#define DIGICLOCK_H

#include <QDialog>
#include <QLCDNumber>
#include <QTime>
#include <QMouseEvent>
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class Clock : public QLCDNumber
{
    Q_OBJECT

public:
    bool firstTime = true;
    void pause();
    void restart();
    Clock(QWidget *parent = nullptr);
    ~Clock();
    void mousePressEvent(QMouseEvent *event) override;
public slots:
    int remainTime();
    void setRemainTime(int value);
private slots:
    void showTime();
signals:
    void timesUp();
    void isPaused();
private:
    int sec = 302;
    void setSec(int sec);
    QTimer *timer;

};
#endif // DIGICLOCK_H
