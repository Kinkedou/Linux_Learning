#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "led.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labelHumi=this->findChild<QLabel*>("label");
    labelTemp=this->findChild<QLabel*>("label_2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QLabel *MainWindow::GetHumiLabel()
{
    return labelHumi;
}
QLabel *MainWindow::GetTempLabel()
{
    return labelTemp;
}

void MainWindow:: on_pushButton_clicked()
{
    static int status=1;
    if(status)
    {
        qDebug()<<"LED cilcked on";
    }
    else
        qDebug()<<"LED clicked off";
    led_control(status);
    status=!status;
}

