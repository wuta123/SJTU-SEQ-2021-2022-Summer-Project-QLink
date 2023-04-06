#pragma once
#ifndef HELPFORSINGLE_H
#define HELPFORSINGLE_H

#include <QMainWindow>
#include "returnbutton.h"
#include "singlemode.h"
#include "singlemodet.h"
#include "mapedit.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPaintDevice>
#include <QFile>
class HelpForSingle : public QMainWindow
{
    Q_OBJECT
public:
    explicit HelpForSingle(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
private:
    QFile *reader;
    returnButton rButton;
    SingleMode *sGame = nullptr;
    SingleModeT *sGameT = nullptr;
    MapEdit *mapEdit = nullptr;
    QPushButton *btn;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    void judgeButtonAvaliablity();
    void connectButtonFunction();
protected:
signals:
    void rSignal();
public slots:
    
};

#endif // HELPFORSINGLE_H
