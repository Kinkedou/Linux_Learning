#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rpc_client.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labelHumi = this->findChild<QLabel*>("label");
    labelTemp = this->findChild<QLabel*>("label_2");
}

QLabel *MainWindow::GetHumiLabel()
{
    return labelHumi;
}

QLabel *MainWindow::GetTempLabel()
{
    return labelTemp;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    static int status = 1;

    if (status)
        qDebug()<<"LED clicked on";
    else
        qDebug()<<"LED clicked off";

    /* 2. control LED */
    rpc_led_control(status);

    status = !status;
}
