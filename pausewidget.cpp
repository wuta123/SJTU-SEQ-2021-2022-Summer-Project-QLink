#include "pausewidget.h"
#include "ui_pausewidget.h"
#include <QPalette>
#include <QPainter>
/*暂停窗口为一个以stackedwidget为主体的窗体，设置页面、暂停页面、战斗胜利页面以及战斗失败页面都被
 * 放在了其中的不同页，并在需要的时候被调用
 * 其中包括用于返回正在进行的游戏的resumeButton、用于退回标题界面的returnBackButton、打开设置页面的
 * settingButton、用于保存游戏的saveButton等*/
PauseWidget::PauseWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PauseWidget)
{
    ui->setupUi(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    QFont font("微软雅黑",15);
    ui->stackedWidget->setCurrentIndex(0);
    ui->title->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    ui->title->setFont(font);
    ui->title->setPalette(pa);
    connect(ui->resumeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->resumeButton, &QPushButton::clicked, this, &PauseWidget::resume);
    connect(ui->returnBackButton, &QPushButton::clicked, this, &PauseWidget::returnToBackground);
    connect(ui->returnPageButton, &QPushButton::clicked, this, [=](){
        settingCompleted();
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->settingButton, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    ui->voiceRadioButton->setChecked(true);
    ui->voiceRadioButton->setCheckable(true);
    setAutoFillBackground(true);
    QPixmap *backGround = new QPixmap;
    backGround->fill(Qt::white);
    QPainter *painter = new QPainter(this);
    painter->drawPixmap(this->rect(),*backGround);
    connect(ui->ReturnButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->ReturnButton, &QPushButton::clicked, this, &PauseWidget::returnToBackground);
    connect(ui->ReturnButton_2, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->ReturnButton_2, &QPushButton::clicked, this, &PauseWidget::returnToBackground);
    connect(ui->saveButton, &QPushButton::clicked, this, &PauseWidget::savegame);
}

void PauseWidget::settingCompleted()
{
    /*在退出暂停窗口时调用*/
    if(ui->voiceRadioButton->isChecked())
        emit turnOnVolumn();
    else if(!ui->voiceRadioButton->isChecked())
        emit turnOffVolumn();
}

void PauseWidget::setMarksForVictory(int marks)
{
    /*战斗胜利后调用*/
    ui->stackedWidget->setCurrentIndex(2);
    ui->Marks->setText(QString::number(marks));
    this->show();
    victoryMusic->setLoops(1);
    victoryMusic->play();
}

void PauseWidget::setMarksForFailure(int marks)
{
    /*战斗失败后调用*/
    ui->stackedWidget->setCurrentIndex(3);
    ui->Marks_2->setText(QString::number(marks));
    this->show();
    failureMusic->setLoops(1);
    failureMusic->play();
}
