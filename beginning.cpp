#include "beginning.h"
#include "singlegamestartbutton.h"
#include "doublegamestartbutton.h"
#include <QTime>
#include <QCoreApplication>

Beginning::Beginning(QWidget *parent):MainWindow(parent)/*主要是实现场景中的单人、双人以及退出按钮的功能*/
{
    this->setWindowTitle(QStringLiteral("开始场景"));
    begin = new singlegamestartbutton("",this);
    begin->move(620,400);
    begin->resize(300,300);

    begin2 = new doublegamestartbutton("",this);
    begin2->move(765,400);
    begin2->resize(300,300);
    begin2->hide();
    qButton = new QPushButton("",this);
    qButton->setStyleSheet("QPushButton{border-image: url(:/image/image/quitButton.png);}"
                            "QPushButton:hover{border-image: url(:/image/image/quitHanging.png);}"
                            "QPushButton:pressed{border-image: url(:/image/image/quitPressed.png);}");
    qButton->resize(100,100);
    qButton->move(1500-qButton->width(),850-qButton->height());
    connect(qButton,&QPushButton::clicked,this,&QMainWindow::close);

    connect(begin,&singlegamestartbutton::clicked,[=](){
        //隐藏当前窗口，显示帮助窗口
        sHelp = new HelpForSingle;
        sHelp->resize(this->size());
        sHelp->move(this->pos());
        connect(sHelp,&HelpForSingle::rSignal,this,&QMainWindow::show);
        this->sHelp->show();
        this->hide();
    });
    connect(begin2,&doublegamestartbutton::clicked,[=](){
        dHelp = new HelpForDouble;
        QTime t;
        this->hide();
        this->dHelp->show();
    });


}
