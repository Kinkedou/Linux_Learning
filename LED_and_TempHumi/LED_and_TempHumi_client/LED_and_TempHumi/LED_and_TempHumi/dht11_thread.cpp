#include <QThread>
#include <QDebug>
#include "dht11_thread.h"
#include "rpc_client.h"

// 2. 重写run()函数，这里是线程要执行的代码
void DHT11Thread::run()
{
    char humi;
    char temp;

    char buf[20];


    while(1) {
        if (0 == rpc_dht11_read(&humi, &temp))
        {
            /* display on qt */
            sprintf(buf, "%d%%", humi);
            labelHumi->setText(buf);
            sprintf(buf, "%d", temp);
            labelTemp->setText(buf);

            msleep(1000); // 线程休眠1秒
        }
    }
}

void DHT11Thread::SetLables(QLabel *labelHumi, QLabel *labelTemp)
{
    this->labelHumi = labelHumi;
    this->labelTemp = labelTemp;
}

