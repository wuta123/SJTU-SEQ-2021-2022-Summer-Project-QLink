#pragma once
#ifndef RETURNBUTTON_H
#define RETURNBUTTON_H

#include <QPushButton>

class returnButton : public QPushButton
{
public:
    returnButton();
    returnButton(QWidget *parent);
signals:
    void returns();

};

#endif // RETURNBUTTON_H
