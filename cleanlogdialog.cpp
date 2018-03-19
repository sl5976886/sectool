#include "cleanlogdialog.h"
#include "ui_cleanlogdialog.h"
#include "commonfunction.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

cleanLogDialog::cleanLogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cleanLogDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("清理日志"));
    this->setFixedSize(this->width(),this->height());
    ui->dateTimeEdit->setCalendarPopup(true);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

cleanLogDialog::~cleanLogDialog()
{
    delete ui;
}

void cleanLogDialog::on_certainButton_clicked()
{
    char strTime[128] = { 0 };
    time_t time = ui->dateTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time,strTime,128);
    QString strTitle = QString("清理");
    QString strMsg = QString("你确定清理操作日志吗？");
    QString showMsg = "<font color='black'>"+strMsg+"</font>";
    QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok|QMessageBox::No,this);
    if(NULL!=WrrMsg->button(QMessageBox::Ok))
    {
        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
    }
    if(NULL!=WrrMsg->button(QMessageBox::No))
    {
        WrrMsg->button(QMessageBox::No)->setText(QString("取消"));
    }
    int ok = WrrMsg->exec();

    if(ok == QMessageBox::No)
    {
    }
    else
    {
        QSqlQuery query;
        QString strSql = QString("DELETE FROM operate_log WHERE operate_time < '%1'").arg(strTime);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql;
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
        }
        else
        {
            QString strTitle = QString("成功");
            QString strMsg = QString("操作成功");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            this->close();
        }
    }
}
