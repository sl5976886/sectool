#include "addpersondialog.h"
#include "ui_addpersondialog.h"
#include "commonfunction.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

addPersonDialog::addPersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPersonDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("添加应急处置人员"));
    this->setFixedSize(this->width(),this->height());
}

addPersonDialog::~addPersonDialog()
{
    delete ui;
}

void addPersonDialog::on_pushButton_clicked()
{
    if(ui->nameLineEdit->text().length()==0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("姓名不能为空！"));
        ui->nameLineEdit->setFocus();
    }
    else if(ui->numLineEdit->text().length()==0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("警号不能为空！"));
        ui->numLineEdit->setFocus();
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("成功"),QString::fromLocal8Bit("人员已添加！"));
        this->close();
    }
}
