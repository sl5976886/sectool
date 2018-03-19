#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QSize>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(QString::fromLocal8Bit("应急处置工具"));
    ui->mainTabWidget->setTabText(0,QString::fromLocal8Bit("首页"));
    ui->mainTabWidget->setTabText(1,QString::fromLocal8Bit("任务管理"));
    ui->mainTabWidget->setTabText(2,QString::fromLocal8Bit("应急处置"));
    ui->mainTabWidget->setTabText(3,QString::fromLocal8Bit("处置报告"));
    ui->otherLabel->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
