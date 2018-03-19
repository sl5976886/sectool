#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "logindialog.h"
#include "commonfunction.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QByteArray>
#include <QCryptographicHash>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    qInstallMessageHandler(outputMessage);
    this->setWindowTitle(QString("注册用户"));
    this->setFixedSize(this->width(),this->height());

    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));

    QSqlQuery query;
    QString strSql = QString("SELECT ORGNAME FROM cfg_orgcode WHERE PARENT=0 OR PARENT=1");
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
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_registerButton_clicked()
{
    QString strOrg = ui->comboBox->currentText();
    if(strOrg.isEmpty())
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择检查单位");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    if(ui->userNameLineEdit->text().length()==0)
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
        ui->userNameLineEdit->setFocus();
        return;
    }
    else if(ui->userNameLineEdit->text().length()<3||ui->userNameLineEdit->text().length()>20)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("用户名长度限制3-20位，请重新输入");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->userNameLineEdit->setFocus();
        return;
    }
    else if(ui->passwdLineEdit->text().length()==0)
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
        ui->passwdLineEdit->setFocus();
        return;
    }
    else if(ui->passwdLineEdit->text().length()>20)
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
        ui->passwdLineEdit->setFocus();
        return;
    }
    else if(ui->passwdAgainLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请重复输入密码");
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
    else if(ui->passwdAgainLineEdit->text()!=ui->passwdLineEdit->text())
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
        ui->passwdAgainLineEdit->setFocus();
        return;
    }
    else if(ui->nameLineEdit->text().length()>20)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("姓名长度超出限制");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->nameLineEdit->setFocus();
        return;
    }
    else if(ui->dutyLineEdit->text().length()>20)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("职务长度超出限制");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->dutyLineEdit->setFocus();
        return;
    }
    else
    {
        QSqlQuery query;
        QString strSql = QString("SELECT id FROM register_user WHERE user_name = '%1'")
                .arg(ui->userNameLineEdit->text());
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
        int count = query.size();
        if(count>0)
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
        QByteArray pass_array;
        pass_array.append(ui->passwdLineEdit->text());
        QByteArray hash_byte_array = QCryptographicHash::hash(pass_array, QCryptographicHash::Md5);
        QString passMd5 = hash_byte_array.toHex();
        strSql = QString("SELECT ORGCODE FROM cfg_orgcode WHERE ORGNAME='%1'").arg(strOrg);
        QString orgCode;
        ok = query.exec(strSql);
        if(ok)
        {
            if(query.next())
            {
                orgCode = query.value(0).toString();
            }
        }
        strSql = QString("INSERT INTO register_user(user_name,passwd,employer,poliNum,true_name,"
                                                "duty,phone,ORGNAME,ORGCODE) VALUES('%1','%2','%3','%4','%5','%6','%7','%8','%9')")
                .arg(ui->userNameLineEdit->text()).arg(passMd5).arg(ui->comboBox->currentText())
                .arg(ui->poliNumLineEdit->text()).arg(ui->nameLineEdit->text()).arg(ui->dutyLineEdit->text())
                .arg(ui->phoneLineEdit->text()).arg(strOrg).arg(orgCode);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
            QString strTitle = QString("警告");
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
        strSql = QString("SELECT id FROM register_user WHERE user_name = '%1'")
                        .arg(ui->userNameLineEdit->text());
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
            QString strTitle = QString("警告");
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
        int userId;
        if(query.next())
        {
            userId = query.value(0).toInt();
        }
        if(ui->questionComboBox->currentText().length()!=0)
        {
            strSql = QString("INSERT INTO security_question(id,question_one,answer_one,question_two,answer_two,"
                                            "question_three,answer_three) "
                                            "VALUES (%1,'%2','%3','%4','%5','%6','%7')").arg(userId)
                    .arg(ui->questionComboBox->currentText()).arg(ui->questionLineEdit1->text())
                    .arg(ui->questionComboBox2->currentText()).arg(ui->questionLineEdit2->text())
                    .arg(ui->questionComboBox3->currentText()).arg(ui->questionLineEdit3->text());
            ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
                QString strTitle = QString("警告");
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
        }
        QString strTitle = QString("成功");
        QString strMsg = QString("已成功注册用户");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        this->close();
//        LoginDialog ld(this);
//        ld.exec();
    }
}

