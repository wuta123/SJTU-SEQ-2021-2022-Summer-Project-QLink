#pragma once
#ifndef CHARASELECTION_H
#define CHARASELECTION_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QPushButton>

class charaSelection: public QWidget
{
    Q_OBJECT
public:
    charaSelection(QWidget *parent = 0);
    ~charaSelection() = default;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPushButton chara1;
    QPushButton chara2;
    QPushButton chara3;
    QPushButton chara4;
signals:
    void chara1Selected();
    void chara2Selected();
    void chara3Selected();
    void chara4Selected();
private:
    void loadingChara();
};

#endif // CHARASELECTION_H
