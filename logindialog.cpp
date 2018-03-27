#include "logindialog.h"
#include "ui_logindialog.h"
#include "commonfunction.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>
#include <QFile>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{

    ui->setupUi(this);
    this->setWindowTitle(QString("登录"));
    qInstallMessageHandler(outputMessage);
    this->setFixedSize(this->width(),this->height());
//    ui->LoginPushButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 17pt Arial Rounded MT Bold ;border-radius:5px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");
    ui->userLineEdit->setStyleSheet("border:2px groove gray;"
                                    "border-radius:3px");
    ui->passwdLineEdit->setStyleSheet("border:2px groove gray;border-radius:3px");

    mw = new mainDialog(parent);
    initStyle();

    QString style = QString("QPushButton{border:none;background-color:#444444;}"
                            "QPushButton:hover{border:none;background-color:#444444;}"
                            "QPushButton:pressed{border:none;background-color:#444444;}");
    ui->forgetPushButton->setStyleSheet(style);
    ui->pushButton->setStyleSheet(style);
    //mw->setAttribute(Qt::WA_DeleteOnClose, true);
}

LoginDialog::~LoginDialog()
{
    //qDebug()<<"LoginDialog uninit";
    delete ui;
}

void LoginDialog::initStyle()
{
    //加载样式表
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

//注册按钮
void LoginDialog::on_pushButton_clicked()
{
    RegisterDialog rw(this);
    rw.setModal(true);
    rw.show();
    rw.exec();
}

//判断账号是否锁定
bool LoginDialog::isLocked()
{
    QSqlQuery query;
    QString strSql;
    strSql = QString("SELECT * FROM sys_set WHERE user_name='%1'").arg(ui->userLineEdit->text());
    bool ok = query.exec(strSql);
    if(!ok)
    {
        QMessageBox::warning(this,QString("警告"),QString("数据库出错"),
                             QMessageBox::Yes);
        qWarning()<<"sql err:"<<query.lastError();
        return true;
    }
    int num = query.size();
    if(num==0)
    {
        strSql = QString("INSERT INTO sys_set SET user_name='%1',wrong_num=3,lock_time=600,max_num=10000,"
                                        "account_lock_time=300").arg(ui->userLineEdit->text());
        QSqlQuery query1;
        query1.exec(strSql);
    }
    if(query.next())
    {
        int errNum = query.value(1).toInt();
        int errTimes = query.value(5).toInt();
        QDateTime lockTime = query.value(6).toDateTime();
        int needLockTime = query.value(4).toInt();
        QDateTime curTime = QDateTime::currentDateTime();
        time_t time = curTime.toTime_t();
        time_t tLockTime = lockTime.toTime_t();
        if(errTimes>errNum)
        {
            QSqlQuery query1;

            char strLastTime[128];
            GetAsciiTime(time,strLastTime,128);

            strSql = QString("UPDATE sys_set SET wrong_times=0,last_lock_time='%1' WHERE user_name='%2'")
                    .arg(strLastTime).arg(ui->userLineEdit->text());
            bool ok = query1.exec(strSql);
            if(!ok)
            {
                QMessageBox::warning(this,QString("警告"),QString("数据库出错"),
                                     QMessageBox::Yes);
                qWarning()<<"sql err:"<<query.lastError();
            }
            QMessageBox::warning(this,QString("警告"),QString("已超出最大尝试次数，账户已被锁定"),
                                 QMessageBox::Yes);
            return true;
        }
        if(time<(tLockTime+needLockTime))
        {
            QMessageBox::warning(this,QString("警告"),QString("账户已被锁定，请稍后尝试"),
                                 QMessageBox::Yes);
            return true;
        }
    }
    return false;
}

//登录按钮
void LoginDialog::on_LoginPushButton_clicked()
{
    if(ui->userLineEdit->text().length()==0)
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
//        QMessageBox::warning(this,QString("警告"),QString("请输入用户名"),
//                             QMessageBox::Yes);
        ui->userLineEdit->setFocus();
        return;
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
        return;
    }
    if(isLocked())
    {
        return;
    }
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM register_user WHERE user_name = '%1'")
            .arg(ui->userLineEdit->text());
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error:"<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
        QString strTitle = QString("错误");
        QString strMsg = QString("数据库错误");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        return;
    }
    int count = query.size();
    if(count==0)
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("用户不存在！");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        return;
    }
    if(query.next())
    {
        QByteArray pass_array;
        pass_array.append(ui->passwdLineEdit->text());
        QByteArray hash_byte_array = QCryptographicHash::hash(pass_array, QCryptographicHash::Md5);
        QString passMd5 = hash_byte_array.toHex();
        QString password = query.value(2).toString();
//        qDebug()<<"md5: "<<passMd5<<"  pass:"<<password;
        if(password!=passMd5)
        {
            QString strTitle = QString("错误");
            QString strMsg = QString("用户名或密码错误！");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))

            {

                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

            }
            WrrMsg->exec();
            QSqlQuery query1;
            QString strSql1;
            strSql1 = QString("SELECT * FROM sys_set WHERE user_name = '%1'").arg(ui->userLineEdit->text());
            bool ok = query1.exec(strSql1);
            if(!ok)
            {
                qWarning()<<"sql err: "<<query1.lastError();
                QString strTitle = QString("错误");
                QString strMsg = QString("数据库出错！");
                QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                if(NULL!=WrrMsg->button(QMessageBox::Ok))

                {

                    WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

                }
                WrrMsg->exec();
            }
            int count = query1.size();
            if(count==0)
            {
                qWarning()<<"sys_set query num is error!";
            }
            else if(count==1)
            {
                if(query1.next())
                {
                    int wrongTimes = query1.value(5).toInt();
                    ++wrongTimes;
                    QSqlQuery query2;
                    strSql1 = QString("UPDATE sys_set SET wrong_times = %1 WHERE user_name = '%2'").arg(wrongTimes)
                            .arg(ui->userLineEdit->text());
                    ok = query2.exec(strSql1);
                    if(!ok)
                    {
                        qWarning()<<"sql err: "<<query2.lastError();
                        QString strTitle = QString("错误");
                        QString strMsg = QString("数据库出错！");
                        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                        if(NULL!=WrrMsg->button(QMessageBox::Ok))

                        {

                            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

                        }
                        WrrMsg->exec();
                    }
                }
            }
            else
            {
                qWarning()<<"query sys_log num error!";
                QString strTitle = QString("错误");
                QString strMsg = QString("数据库出错！");
                QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                if(NULL!=WrrMsg->button(QMessageBox::Ok))

                {

                    WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

                }
                WrrMsg->exec();
            }
            return;
        }
        QString state = query.value(9).toString();
        QString strState = QString("正常");
        if(state!=strState)
        {
            QString strTitle = QString("警告");
            QString strMsg = QString("该用户已停止使用！");
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
    g_user_name = ui->userLineEdit->text();
    QString action = QString("用户登录");
    QString state = QString("成功");
    operateLog(action,state);

    strSql = QString("UPDATE sys_set SET wrong_times=0 WHERE user_name='%1'").arg(g_user_name);
    ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err:"<<query.lastError();
    }
    this->hide();
    mw->show();
    mw->exec();
}


//忘记密码按钮
void LoginDialog::on_forgetPushButton_clicked()
{
    ForgetDialog fw(this);
    fw.setModal(true);
    fw.show();
    fw.exec();
}
