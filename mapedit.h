#pragma once
#ifndef MAPEDIT_H
#define MAPEDIT_H

#include <QWidget>

namespace Ui {
class MapEdit;
}

class MapEdit : public QWidget
{
    Q_OBJECT

public:
    explicit MapEdit(QWidget *parent = nullptr);
    ~MapEdit();
signals:
    void startGame(int x, int y, int firstTime);
private:
    void tryToStart();
    void load();
    Ui::MapEdit *ui;
};

#endif // MAPEDIT_H
