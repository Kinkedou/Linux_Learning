#ifndef DHT11_THREAD_H
#define DHT11_THREAD_H
#include <QThread>
#include <QLabel>

// 1. 创建继承自QThread的子类
class DHT11Thread : public QThread {
public:
    void run() override ;
    void SetLables(QLabel *labelHumi, QLabel *labelTemp);
private:
    QLabel *labelHumi;
    QLabel *labelTemp;
};

#endif // DHT11_THREAD_H
