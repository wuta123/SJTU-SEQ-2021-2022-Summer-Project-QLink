#pragma once
#ifndef DOUBLEGAMESTARTBUTTON_H
#define DOUBLEGAMESTARTBUTTON_H

#include <QPushButton>
#include <QPainter>

class doublegamestartbutton : public QPushButton
{
public:
    doublegamestartbutton();
    doublegamestartbutton(const QString &text, QWidget *parent = nullptr);
public slots:
    void enteredEvent();
    void leavedEvent();
    void pressedEvent();
    void releasedEvent();
};

#endif // DOUBLEGAMESTARTBUTTON_H
