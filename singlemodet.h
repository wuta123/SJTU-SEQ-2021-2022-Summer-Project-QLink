#pragma once
#ifndef SINGLEMODET_H
#define SINGLEMODET_H

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QLabel>
#include <QTime>
#include <QMouseEvent>
#include "returnbutton.h"
#include "playert.h"
#include <QProgressBar>
#include <QDockWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QLabel>
#include <QWidget>
#include <QTime>
#include <QFileInfo>
#include <stdio.h>
#include <QMainWindow>
#include "returnbutton.h"
#include <QPushButton>
#include <QBitmap>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>
#include <QFontDatabase>
#include <QPropertyAnimation>
#include <cmath>
#include <QFile>
#include <cstring>
#include <string>
#include <QMessageBox>
#include <QTextStream>
#include "clock.h"
#include "blockst.h"
#include "pausewidget.h"
#include <QStack>
#include <QQueue>
#include <QLineF>
namespace Ui {
class SingleModeT;
}

typedef BlocksT* block;

class SingleModeT : public QWidget
{
    Q_OBJECT

public:
    explicit SingleModeT(int x, int y, int firstTime);//x,y分别表示在这张地图中生成一个x*y大小的地图
    void setMap(int x, int y, int firstTime);//从生成x*y大小的地图，常用
    void pause();
    ~SingleModeT();
    void addMark(int num);
public slots:
    void saveGame();
    void loadGame();//从文件中保存和读取游戏数据
    
private slots:
    void startProgress();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void returnToHomepage();
private:
    int X,Y;
    PlayerT player;
    int carriedCode = 999;
    int carriedState = 999;//携带物的种类
    int marks = 0;
    int Blocksnum;
    QGraphicsScene *scene = nullptr;
    QPixmap *background = nullptr;
    returnButton *btnBack = nullptr;
    int step;
    block* blocks;
    int **matrix;
    int flashCount = 5;
    QTimer *FlashTimer = nullptr;
    bool paused = false;
    bool failed = false;
    bool ableToFlash = false;
    QPointF *middlePoint = new QPointF(385,310);
    void addBlocks(int x, int y);
    void setClocks();
    void showFail();
    void showVictory();
    void judgeEliminate(int code);
    bool judgeCollide();
    void shuffle();
    void flash();
    void addTime();
    Ui::SingleModeT *ui;
    void setRules();
    bool ableToLink(int carriedCode, int code);
    void drawLine();//画线操作
    QPointF physicalPoint(int originPointx, int originPointy);
    bool isBlock(int x, int y);
    int min(int a, int b){
        return a < b ? a:b;
    }
    int max(int a, int b){
        return a > b ? a:b;
    }
    void showMatrix();
    bool horizontal(int x1, int y1, int x2, int y2);
    bool vertical(int x1, int y1, int x2, int y2);
    bool oneTurn(int x1, int y1, int x2, int y2);
    bool twoTurn(int x1, int y1, int x2, int y2);
    QQueue<QPointF> keypoints;
    void findTheSame();
};

#endif // SINGLEMODET_H
