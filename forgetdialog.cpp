#include "forgetdialog.h"
#include "ui_forgetdialog.h"
#include "logindialog.h"
#include "commonfunction.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QByteArray>
#include <QCryptographicHash>

ForgetDialog::ForgetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("忘记密码"));
    this->setFixedSize(this->width(),this->height());
    qInstallMessageHandler(outputMessage);
}

ForgetDialog::~ForgetDialog()
{
    delete ui;
}


//确定按钮
void ForgetDialog::on_okButton_clicked()
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
        ui->passwdLineEdit->setFocus();
        return;
    }
    else if(ui->passwdLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请输入新密码");
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
    else if(ui->passwdLineEdit->text().length()<8)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("密码长度不够");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    else if(ui->passwdLineEdit->text().length()>16)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("密码长度超出限制");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    else if(ui->passwdAgainLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请重复输入新密码");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->passwdAgainLineEdit->setFocus();
        return;
    }
    else if(ui->passwdLineEdit->text()!=ui->passwdAgainLineEdit->text())
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("两次输入的密码不一致");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    else
    {
        qDebug()<<"reset password,useName: "<<ui->userLineEdit->text().toStdString().c_str();
        QSqlQuery query;
        QString strSql = QString("SELECT id FROM register_user WHERE user_name = '%1'")
                .arg(ui->userLineEdit->text());
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
        int row = query.size();
        if(row==0)
        {
            QString strTitle = QString("警告");
            QString strMsg = QString("不存在的用户");
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
            int id = query.value(0).toInt();
            strSql = QString("SELECT * FROM security_question WHERE id = %1").arg(id);
            ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
                QString strTitle = QString("错误");
                QString strMsg = QString("数据库出错");
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
                QString ques_1 = query.value(1).toString();
                QString ans_1 = query.value(2).toString();
                QString ques_2 = query.value(3).toString();
                QString ans_2 = query.value(4).toString();
                QString ques_3 = query.value(5).toString();
                QString ans_3 = query.value(6).toString();
                int flag = 0;
                if(ques_1==ui->questionComboBox->currentText()&&ans_1==ui->questionLineEdit->text()&&ans_1.length()!=0)
                {
                    flag = 1;
                }
                if(ques_2==ui->questionComboBox->currentText()&&ans_2==ui->questionLineEdit->text()&&ans_2.length()!=0)
                {
                    flag = 1;
                }
                if(ques_3==ui->questionComboBox->currentText()&&ans_3==ui->questionLineEdit->text()&&ans_3.length()!=0)
                {
                    flag = 1;
                }
                if(flag==1)
                {
                    QByteArray pass_array;
                    pass_array.append(ui->passwdLineEdit->text());
                    QByteArray hash_byte_array = QCryptographicHash::hash(pass_array, QCryptographicHash::Md5);
                    QString passMd5 = hash_byte_array.toHex();
                    strSql = QString("UPDATE register_user SET passwd = '%1'")
                            .arg(passMd5);
                    ok = query.exec(strSql);
                    if(!ok)
                    {
                        qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
                        QString strTitle = QString("错误");
                        QString strMsg = QString("数据库出错");
                        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                        if(NULL!=WrrMsg->button(QMessageBox::Ok))
                        {
                            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                        }
                        WrrMsg->exec();
                        return;
                    }
                    QString action = QString("修改密码");
                    QString state = QString("成功");
                    operateLog(action,state);
                    QString strTitle = QString("成功");
                    QString strMsg = QString("修改密码成功");
                    QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                    if(NULL!=WrrMsg->button(QMessageBox::Ok))
                    {
                        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                    }
                    WrrMsg->exec();
                    //LoginDialog ld(this);
                    this->close();
                    //ld.exec();
                    return;
                }
                else
                {
                    QString strTitle = QString("警告");
                    QString strMsg = QString("安全问题错误");
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
        }
    }
    return;
}


