#include "doublegamestartbutton.h"
#include <QPropertyAnimation>

doublegamestartbutton::doublegamestartbutton()
{

}

doublegamestartbutton::doublegamestartbutton(const QString &text, QWidget *parent)
{
    this->setText(text);
    this->setParent(parent);
    this->setStyleSheet("QPushButton{border-image: url(:/image/image/doubleMode.png);}"
                        "QPushButton:hover{border-image: url(:/image/image/doubleModeEntered.png);}"
                        "QPushButton:pressed{border-image: url(:/image/image/doubleModePressed.png);}");
}

