#pragma once
#ifndef PAUSEWIDGET_H
#define PAUSEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QSound>

namespace Ui {
    class PauseWidget;
}
class PauseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PauseWidget(QWidget *parent = nullptr);

signals:
    void resume();
    void returnToBackground();
    void turnOnVolumn();
    void turnOffVolumn();
    void savegame();
public slots:
    void settingCompleted();
    void setMarksForVictory(int marks);
    void setMarksForFailure(int marks);
private:
    Ui::PauseWidget *ui;
    QSound *victoryMusic = new QSound(":/soundEffects/music/ShowVictory.wav", this);
    QSound *failureMusic = new QSound(":/soundEffects/music/ShowFail.wav", this);
};

#endif // PAUSEWIDGET_H
