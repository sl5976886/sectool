#include "newuserdialog.h"
#include "ui_newuserdialog.h"
#include "commonfunction.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QLayout>
#include <QByteArray>
#include <QCryptographicHash>
#include "sysmanagedialog.h"

newUserDialog::newUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newUserDialog)
{
    ui->setupUi(this);
//    ui->cancelButton->setStyleSheet("QPushButton{background-color:rgb(240,247,253);color:black;border: 1px solid rgb(222,227,232);}"
//                             "QPushButton:hover{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(52, 194, 250, 255), stop:1 rgba(16, 144, 248, 255));color:white;}"
//                             "QPushButton:pressed{background-color:rgb(16,144,248);color:white;border-style: inset;}");
    this->setWindowTitle(QString("新增用户"));
    this->setFixedSize(this->width(),this->height());
    comBoxInit();
    //this->setStyleSheet("background-color:rgb(240,240,240)");

    certainButton = new QPushButton(QString("确定"),this);
//    certainButton->setStyleSheet("QPushButton{background-color:rgb(240,247,253);color:black;border: 1px solid rgb(222,227,232);width:75px;height:23px;}"
//                             "QPushButton:hover{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(52, 194, 250, 255), stop:1 rgba(16, 144, 248, 255));color:white;width:75px;height:23px;}"
//                             "QPushButton:pressed{background-color:rgb(16,144,248);color:white;border-style: inset;width:75px;height:23px;}");

    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);

    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(on_determineButton_clicked()));
}

newUserDialog::~newUserDialog()
{
    delete ui;
}

void newUserDialog::on_determineButton_clicked()
{
    int ifSuccess = 0;
    if(ui->nameLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请输入用户名");
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
    else if(ui->nameLineEdit->text().length()<3||ui->nameLineEdit->text().length()>20)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("用户名长度限制为3-20位");
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
    else
    {
        QSqlQuery query1;
        QString strSql1 = QString("SELECT id FROM register_user WHERE user_name = \'%1\'")
                .arg(ui->nameLineEdit->text());
        bool ok1=query1.exec(strSql1);
        if(!ok1)
        {
            qDebug()<<query1.lastError();
        }
        else if(query1.next()&&!ifDispose)
        {
            QString strTitle = QString("警告");
            QString strMsg = QString("已存在该用户");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
    }
    if(ui->passwdLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请输入密码");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->passwdLineEdit->setFocus();
        ifSuccess=1;
        return;
    }
    if(ui->passwdLineEdit->text().length()<8||ui->passwdLineEdit->text().length()>16)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("密码长度限制为8-16位");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->passwdLineEdit->setFocus();
        ifSuccess=1;
        return;
    }
    if(ui->passwdLineEdit_2->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请重复一遍密码");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->passwdLineEdit_2->setFocus();
        ifSuccess=1;
        return;
    }
    if(ui->passwdLineEdit->text()!=ui->passwdLineEdit_2->text())
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("两次输入的密码不相同");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->passwdLineEdit_2->setFocus();
        ifSuccess=1;
        return;
    }
    if(ui->trueNameLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请输入真实姓名");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->trueNameLineEdit->setFocus();
        ifSuccess=1;
        return;
    }
    if(ui->policeNumLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请输入警号");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->policeNumLineEdit->setFocus();
        ifSuccess=1;
        return;
    }
    QByteArray pass_array;
    pass_array.append(ui->passwdLineEdit->text());
    QByteArray hash_byte_array = QCryptographicHash::hash(pass_array, QCryptographicHash::Md5);
    QString passMd5 = hash_byte_array.toHex();
    if(ifSuccess==0)
    {
        QSqlQuery query;
        QString strSql;
        if(ifDispose)
        {
            strSql = QString("INSERT INTO dispose_person(name,police_num,check_company,phone,position,task_id)"
                                     "VALUES(\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',%6)").arg(ui->nameLineEdit->text(),
                                                                                        ui->policeNumLineEdit->text(),
                                                                                        ui->comboBox->currentText(),
                                                                                        ui->phoneLineEdit->text(),
                                                                                        ui->dutyLineEdit->text()).arg(g_task_id);
        }
        else
        {
            strSql = QString("INSERT INTO register_user(user_name,passwd,employer,poliNum,true_name,duty,phone)"
                                     "VALUES(\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\')").arg(ui->nameLineEdit->text(),
                                                                                        passMd5,
                                                                                        ui->comboBox->currentText(),
                                                                                        ui->policeNumLineEdit->text(),
                                                                                        ui->trueNameLineEdit->text(),
                                                                                        ui->dutyLineEdit->text(),
                                                                                        ui->phoneLineEdit->text());
        }

        bool ok = query.exec(strSql);
        ifDispose = false;
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
        }
        else
        {
            QString action = QString("新增用户");
            QString state = QString("成功");
            operateLog(action,state);
            QString strTitle = QString("添加用户");
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

void newUserDialog::comBoxInit()
{
    QSqlQuery query;
    QString strSql = QString("SELECT com_name FROM emergency_company");
    query.exec(strSql);
    ui->comboBox->setFixedHeight(20);
    while(query.next())
    {
        QString name = query.value(0).toString();
        QPixmap pixmap(1, ui->comboBox->height());
        pixmap.fill(Qt::transparent);
        QIcon icon(pixmap);
        ui->comboBox->setIconSize(QSize(1,ui->comboBox->height()));
        ui->comboBox->addItem(icon,name);
    }
//    ui->comboBox->setStyleSheet("QComboBox{border: 1px solid gray;border-radius: 3px;padding: 1px 2px 1px 2px; min-width: 9em;}"
//                                "QComboBox::drop-down{subcontrol-origin: padding;subcontrol-position: top right;"
//                                "width: 20px;border-left-width: 1px;border-left-color: darkgray;border-left-style: solid;"
//                                " border-top-right-radius: 3px; border-bottom-right-radius: 3px;}"
//                                "QComboBox::down-arrow{ image: url(:/combox/image/combox/arrow.png);}");
}



