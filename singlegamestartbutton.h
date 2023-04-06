#pragma once
#ifndef singlegamestartbutton_H
#define singlegamestartbutton_H

#include <QPushButton>
#include <QWidget>

class singlegamestartbutton : public QPushButton
{
public:
    singlegamestartbutton();
    singlegamestartbutton(const QString &text, QWidget *parent = nullptr);
public slots:
    void enteredEvent();
    void clickedEvent();

};

#endif // singlegamestartbutton_H
