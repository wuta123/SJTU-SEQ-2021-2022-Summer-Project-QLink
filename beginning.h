#pragma once
#ifndef BEGINNING_H
#define BEGINNING_H

#include <QMainWindow>
#include "mainwindow.h"
#include "singlegamestartbutton.h"
#include "doublegamestartbutton.h"
#include "helpforsingle.h"
#include "helpfordouble.h"

class Beginning: public MainWindow
{
    Q_OBJECT
public:
    explicit Beginning(QWidget *parent = nullptr);
private:
    singlegamestartbutton *begin = nullptr;
    doublegamestartbutton *begin2 = nullptr;
    QPushButton *qButton;
    QPushButton *lButton;


signals:

public slots:

private:
    HelpForSingle *sHelp;
    HelpForDouble *dHelp;

};

#endif // BEGINNING_H
