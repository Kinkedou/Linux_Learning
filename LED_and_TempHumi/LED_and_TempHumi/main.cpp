#include "mainwindow.h"
#include "led.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //init LED
    led_init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
