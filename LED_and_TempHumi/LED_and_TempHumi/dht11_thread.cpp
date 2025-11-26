#include <QThread>
#include <QDebug>
#include "dht11_thread.h"
#include "dht11.h"

void DHT11Thread::run()
{
    char humi;
    char temp;

    char buf[20];
    dht11_init();
    while(1){
        if (0== dht11_read(&humi,&temp))
        {
            sprintf(buf,"%d%%",humi);
            labelHumi->setText(buf);
            sprintf(buf,"%d",temp);
            labelTemp->setText(buf);
            msleep(1000);
        }

    }
}

void DHT11Thread::SetLabels(QLabel *labelHumi,QLabel *labelTemp)
{
    this->labelHumi=labelHumi;
    this->labelTemp=labelTemp;
}

