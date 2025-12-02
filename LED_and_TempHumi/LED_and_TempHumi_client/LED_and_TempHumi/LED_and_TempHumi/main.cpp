#include "mainwindow.h"
#include "stdio.h"
#include "rpc_client.h"
#include <QApplication>
#include "dht11_thread.h"

int main(int argc, char *argv[])
{
    QLabel *labelHumi;
    QLabel *labelTemp;

    RPC_Client_Init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    labelHumi = w.GetHumiLabel();
    labelTemp = w.GetTempLabel();


    /* create dht11 thread */
    // 3. 实例化线程并启动

    DHT11Thread thread;
    thread.SetLables(labelHumi, labelTemp);
    thread.start();

    return a.exec();
}
