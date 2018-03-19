#include "waitdialog.h"
#include "ui_waitdialog.h"
#include <QMovie>

waitDialog::waitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::waitDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉窗口标题栏

    QMovie *pMovie = new QMovie(":/image/load.gif");
    ui->label->setScaledContents(true);
    pMovie->start();
}

waitDialog::~waitDialog()
{
    delete ui;
}

void waitDialog::finishWait(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    this->close();
}
