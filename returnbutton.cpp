#include "returnbutton.h"

/*因为同一个returnButton被使用了多次，就把它单独拉出来做了一个类，方便使用*/

returnButton::returnButton(){
    this->setStyleSheet("QPushButton{border-image: url(:/image/image/returnButton.png);}"
                            "QPushButton:hover{border-image: url(:/image/image/returnHanging.png);}"
                            "QPushButton:pressed{border-image: url(:/image/image/returnPressed.png);}");
}

returnButton::returnButton(QWidget *parent)
{
    this->setParent(parent);
    this->setStyleSheet("QPushButton{border-image: url(:/image/image/returnButton.png);}"
                            "QPushButton:hover{border-image: url(:/image/image/returnHanging.png);}"
                        "QPushButton:pressed{border-image: url(:/image/image/returnPressed.png);}");
}


