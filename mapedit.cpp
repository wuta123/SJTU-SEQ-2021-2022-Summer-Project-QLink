#include "mapedit.h"
#include "ui_mapedit.h"
#include "QMessageBox"
MapEdit::MapEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapEdit)
{
    ui->setupUi(this);
    ui->xComboBox->addItem("6");
    ui->xComboBox->addItem("7");
    ui->xComboBox->addItem("8");
    ui->xComboBox->addItem("9");
    ui->xComboBox->addItem("10");
    ui->xComboBox->addItem("11");
    ui->xComboBox->addItem("12");
    ui->yComboBox->addItem("6");
    ui->yComboBox->addItem("7");
    ui->yComboBox->addItem("8");
    ui->yComboBox->addItem("9");
    ui->yComboBox->addItem("10");
    ui->yComboBox->addItem("11");
    ui->yComboBox->addItem("12");

    connect(ui->startGameButton, &QPushButton::clicked, this, &MapEdit::tryToStart);
    connect(ui->loadButton, &QPushButton::clicked, this, [=](){
        load();
    });
    connect(ui->returnButton, &QPushButton::clicked, this, [=](){
        close();
    });
}

MapEdit::~MapEdit()
{
    delete ui;
}

void MapEdit::tryToStart()
{
    int x = ui->xComboBox->currentIndex()+6;
    int y = ui->yComboBox->currentIndex()+6;
    if(x*y % 2 != 0){
        QMessageBox::warning(NULL,u8"失败", u8"方块数量不为偶数！");
        return;
    }
    else{
        this->hide();
        emit startGame(x, y, 1);
    }
}

void MapEdit::load()
{
    emit startGame(6,6,0);
    this->hide();
}
