#include "newcomdialog.h"
#include "ui_newcomdialog.h"
#include "commonfunction.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QLayout>

newComDialog::newComDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newComDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("新增单位"));
    this->setFixedSize(this->width(),this->height());
    QString action = QString("新增单位");
    QString state = QString("成功");
    operateLog(action,state);
//    ui->cancelButton->setStyleSheet("QPushButton{background-color:rgb(240,247,253);color:black;border: 1px solid rgb(222,227,232);}"
//                             "QPushButton:hover{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(52, 194, 250, 255), stop:1 rgba(16, 144, 248, 255));color:white;}"
//                             "QPushButton:pressed{background-color:rgb(16,144,248);color:white;border-style: inset;}");
    ui->yesRadioButton->setChecked(true);

    certainButton = new QPushButton(QString("确定"),this);
//    certainButton->setStyleSheet("QPushButton{background-color:rgb(240,247,253);color:black;border: 1px solid rgb(222,227,232);width:75px;height:23px;}"
//                             "QPushButton:hover{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(52, 194, 250, 255), stop:1 rgba(16, 144, 248, 255));color:white;width:75px;height:23px;}"
//                             "QPushButton:pressed{background-color:rgb(16,144,248);color:white;border-style: inset;width:75px;height:23px;}");

    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);
    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(certain_click()));
}

newComDialog::~newComDialog()
{
    delete ui;
}

void newComDialog::certain_click()
{
    int ifSuccess = 0;
    if(ui->nameLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请填写单位名称");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->nameLineEdit->setFocus();
        ifSuccess=1;
        return;
    }
    if(ui->codeLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请填写单位编码");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->codeLineEdit->setFocus();
        ifSuccess=1;
        return;
    }
    if(ui->codeLineEdit->text().length()!=12)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("单位编码为12位数字");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->codeLineEdit->setFocus();
        ifSuccess=1;
        return;
    }
    if(ifSuccess==0)
    {
        QString strState;
        if(ui->yesRadioButton->isChecked())
        {
            strState = QString("启用");
        }
        else if(ui->noRadioButton->isChecked())
        {
            strState = QString("不启用");
        }
        QSqlQuery query;
        QString strSql = QString("INSERT INTO "
                                                "emergency_company(com_name,com_code,com_state)"
                                                "VALUES(\'%1\',\'%2\',\'%3\')").arg(ui->nameLineEdit->text(),
                                                                                    ui->codeLineEdit->text(),
                                                                                    strState);
        //qDebug()<<"sql is:"<<strSql;
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qDebug()<<query.lastError();
        }
        else
        {
            QString strTitle = QString("添加单位");
            QString strMsg = QString("添加成功");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            int i =  WrrMsg->exec();
            if(i == QMessageBox::Ok)
            {
               this->close();
            }
        }
    }
}
