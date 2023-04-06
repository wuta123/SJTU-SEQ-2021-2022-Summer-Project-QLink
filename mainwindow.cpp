#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QPainter>
#include <QPaintDevice>
#include <QPushButton>
#include "singlemode.h"
#include <QDesktopWidget>

//游戏主窗口
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)//设定窗口出现的位置
{
    ui->setupUi(this);

    setFont(QFont("Microsoft Yahei", 9));
    setFixedSize(1600,900);
    //窗口标题
    setWindowTitle(QStringLiteral("连连看"));
    setWindowIcon(QIcon(":/icon/1x1/darkzaitzev/chameleon-glyph.svg"));
    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/3);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)//设置背景; pix背景图， pix2为连连看标题，覆盖在背景图上方
{
    QPainter painter(this);
    QPixmap pix(":/image/image/the_BackGround.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    QPixmap pix2(":/image/image/the_BeginningTitle.png");
    painter.drawPixmap((1600-pix2.width()/2)/2-50,(900-pix2.height())/4,pix2.width(),pix2.height(),pix2);
}

