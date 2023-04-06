#include "singlegamestartbutton.h"

singlegamestartbutton::singlegamestartbutton()
{

}

singlegamestartbutton::singlegamestartbutton(const QString &text, QWidget *parent)
{
    this->setText(text);
    this->setParent(parent);
    this->setStyleSheet("QPushButton{border-image: url(:/image/image/singleMode.png);}"
                            "QPushButton:hover{border-image: url(:/image/image/singleModeEntered.png);}"
                            "QPushButton:pressed{border-image: url(:/image/image/singleModePressed.png);}");
}


