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
    this->setWindowTitle(QString::fromLocal8Bit("Ӧ�����ù���"));
    ui->mainTabWidget->setTabText(0,QString::fromLocal8Bit("��ҳ"));
    ui->mainTabWidget->setTabText(1,QString::fromLocal8Bit("�������"));
    ui->mainTabWidget->setTabText(2,QString::fromLocal8Bit("Ӧ������"));
    ui->mainTabWidget->setTabText(3,QString::fromLocal8Bit("���ñ���"));
    ui->otherLabel->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
