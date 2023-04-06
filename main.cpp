#include "mainwindow.h"
#include "beginning.h"
#include <QApplication>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Beginning b;
    b.show();
    return a.exec();
}
