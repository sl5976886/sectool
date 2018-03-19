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
    this->setWindowTitle(QString::fromLocal8Bit("���Ӧ��������Ա"));
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
        QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("��������Ϊ�գ�"));
        ui->nameLineEdit->setFocus();
    }
    else if(ui->numLineEdit->text().length()==0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("���Ų���Ϊ�գ�"));
        ui->numLineEdit->setFocus();
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("�ɹ�"),QString::fromLocal8Bit("��Ա����ӣ�"));
        this->close();
    }
}
