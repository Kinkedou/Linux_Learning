#include "mainwindow.h"
#include "led.h"
#include <QApplication>
#include "dht11_thread.h"

int main(int argc, char *argv[])
{
    QLabel *labelHumi;
    QLabel *labelTemp;

    //init LED
    led_init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    labelHumi=w.GetHumiLabel();
    labelTemp=w.GetTempLabel();

    //create dht11 thread
    DHT11Thread thread;
    thread.SetLabels(labelHumi,labelTemp);
    thread.start();


    return a.exec();
}
