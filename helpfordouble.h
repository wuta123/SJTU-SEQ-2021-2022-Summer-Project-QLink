#pragma once
#ifndef HELPFORDOUBLE_H
#define HELPFORDOUBLE_H

#include <QMainWindow>

class HelpForDouble : public QMainWindow
{
    Q_OBJECT
public:
    explicit HelpForDouble(QWidget *parent = nullptr);

signals:
    void rSignal();
};

#endif // HELPFORDOUBLE_H
