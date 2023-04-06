#pragma once
#ifndef WORDSOFBOSS_H
#define WORDSOFBOSS_H

#include <QObject>
#include <QLabel>
#include <QPalette>
#include <QMouseEvent>
#include <QFile>
#include <QStringList>
#include <QKeyEvent>
#include <QTimer>
class WordsOfBoss:public QLabel
{
    Q_OBJECT
public:
    WordsOfBoss(QWidget *parent);
    void mousePressEvent(QMouseEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    QString buffer;
    int sentences = 0;//本次发言应该从第几句开始
    int sequence = 0;//记录此时是boss的第几次发言

    int currentNumOfWords = 0;
    int currentSentences();
signals:
    void over();
    void finalOver();//发表遗言
private:

    QTimer *timer;
    const int seq[10] = {4,2,1,2,2,2,2,1,2,1};
    QFile file;
    QStringList wordsList;
public slots:
    void loadSentence();
    void toContinue();
    void say();
    void stop();
    int currentSequece();
    void notFirstTime();
    void loadSequence(int value);
    void loadCurrentSentences(int value);
};

#endif // WORDSOFBOSS_H
