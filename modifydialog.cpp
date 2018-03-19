#include "modifydialog.h"
#include "ui_modifydialog.h"
#include "commonfunction.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>

modifyDialog::modifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifyDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("修改计划"));
    this->setFixedSize(this->width(),this->height());
    ui->dateTimeEdit->setCalendarPopup(true);
    qInstallMessageHandler(outputMessage);

    certainButton = new QPushButton(QString("确定"),this);
    setButtonStyle();
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);
    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(certain_click()));
    tableInit();
}

modifyDialog::~modifyDialog()
{
    delete ui;
}

void modifyDialog::certain_click()
{
    QString str_1 = ui->nameLineEdit->text();
    time_t time_2 = ui->dateTimeEdit->dateTime().toTime_t();
    char strTime[128]={0};
    GetAsciiTime(time_2,strTime,128);
    QString str_3 = ui->leixingComboBox->currentText();
    QString str_4 = ui->jibieComboBox->currentText();
    QString str_5 = ui->danweiComboBox->currentText();
    QString str_6 = ui->textEdit->toPlainText();
    QString str_7 = ui->sysLineEdit->text();
    QString str_8 = ui->levelLineEdit->text();
    QString str_9 = ui->textEdit_2->toPlainText();
    QString str_10 = ui->laiyuanComboBox->currentText();
    QString str_11 = ui->employLineEdit->text();
    QString str_12 = ui->personLineEdit->text();
    QString str_13 = ui->phoneLineEdit->text();
    QString str_14 = ui->areaLineEdit->text();
    QString str_15 = ui->phoneLineEdit_2->text();
    QString str_16 = ui->emailLineEdit->text();

    if(str_1.isEmpty())
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请填写事件名称");
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
    if(str_11.isEmpty())
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请填写事发单位");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->employLineEdit->setFocus();
        return;
    }
    QSqlQuery query;
    QString strSql = QString("UPDATE main_task SET event_company='%1',company_address='%2',principal='%3',"
                                            "phone='%4',office_phone='%5',email='%6',system_name='%7',system_level='%8',"
                                            "event_source='%9',event_name='%10',event_time='%11',event_place='%12',event_kind='%13',"
                                            "event_level='%14',dispose_company='%15',event_discribe='%16' WHERE id=%17").arg(str_11).arg(str_14)
            .arg(str_12).arg(str_15).arg(str_13).arg(str_16).arg(str_7).arg(str_8).arg(str_10).arg(str_1).arg(strTime).arg(str_6)
            .arg(str_3).arg(str_4).arg(str_5).arg(str_9).arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
        return;
    }
    QString strTitle = QString("成功");
    QString strMsg = QString("修改计划成功");
    QString showMsg = "<font color='black'>"+strMsg+"</font>";
    QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
    if(NULL!=WrrMsg->button(QMessageBox::Ok))
    {
        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
    }
    WrrMsg->exec();
    this->close();
}

void modifyDialog::tableInit()
{

    QSqlQuery query;
    QString strSql = QString("SELECT ORGCODE FROM register_user WHERE user_name='%1'").arg(g_user_name);
    QString orgCode;
    bool ok = query.exec(strSql);
    if(ok)
    {
        if(query.next())
        {
            orgCode = query.value(0).toString();
            orgCode = orgCode.mid(0,2);
        }
    }
    else
    {
        qWarning()<<"sql err:"<<query.lastError();
        QString strTitle = QString("错误");
        QString strMsg = QString("获取单位信息失败，请检查数据库连接");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }

    strSql = QString("SELECT ORGNAME FROM cfg_orgcode WHERE ORGCODE LIKE '%1%'").arg(orgCode);
    query.exec(strSql);
    ui->danweiComboBox->setFixedHeight(20);
    while(query.next())
    {
        QString name = query.value(0).toString();
        QPixmap pixmap(1, ui->danweiComboBox->height());
        pixmap.fill(Qt::transparent);
        QIcon icon(pixmap);
        ui->danweiComboBox->setIconSize(QSize(1,ui->danweiComboBox->height()));
        ui->danweiComboBox->addItem(icon,name);
    }

    strSql = QString("SELECT * FROM main_task WHERE id=%1").arg(g_task_id);
    ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    if(query.next())
    {
        QString str_1 = query.value(2).toString();
        QString str_2 = query.value(3).toString();
        QString str_3 = query.value(4).toString();
        QString str_4 = query.value(5).toString();
        QString str_5 = query.value(6).toString();
        QString str_6 = query.value(7).toString();
        QString str_7 = query.value(8).toString();
        QString str_8 = query.value(9).toString();
        QString str_9 = query.value(10).toString();
        QString str_10 = query.value(11).toString();
        QDateTime time_11 = query.value(12).toDateTime();
        QString str_12 = query.value(13).toString();
        QString str_13 = query.value(14).toString();
        QString str_14 = query.value(15).toString();
        QString str_15 = query.value(16).toString();
        QString str_16 = query.value(17).toString();

        ui->nameLineEdit->setText(str_10);
        ui->dateTimeEdit->setDateTime(time_11);
        ui->leixingComboBox->setCurrentText(str_13);
        ui->jibieComboBox->setCurrentText(str_14);
        ui->danweiComboBox->setCurrentText(str_15);
        ui->textEdit->setText(str_12);
        ui->sysLineEdit->setText(str_7);
        ui->levelLineEdit->setText(str_8);
        ui->textEdit_2->setText(str_16);
        ui->laiyuanComboBox->setCurrentText(str_9);
        ui->employLineEdit->setText(str_1);
        ui->personLineEdit->setText(str_3);
        ui->phoneLineEdit->setText(str_5);
        ui->areaLineEdit->setText(str_2);
        ui->phoneLineEdit_2->setText(str_4);
        ui->emailLineEdit->setText(str_6);
    }
}

void modifyDialog::setButtonStyle()
{
//    certainButton->setStyleSheet("QPushButton{-moz-box-shadow:inset 0px 1px 0px 0px #ffffff;"
//                                 "-webkit-box-shadow:inset 0px 1px 0px 0px #ffffff;box-shadow:inset 0px 1px 0px 0px #ffffff;"
//                                 "background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #f9f9f9), color-stop(1, #e9e9e9));"
//                                 "background:-moz-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
//                                 "background:-webkit-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
//                                 "background:-o-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
//                                 "background:-ms-linear-gradient(top, #f9f9f9 5%, #e9e9e9 100%);"
//                                 "background:linear-gradient(to bottom, #f9f9f9 5%, #e9e9e9 100%);"
//                                 "filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#f9f9f9', endColorstr='#e9e9e9',GradientType=0);background-color:#f9f9f9;"
//                                 "-moz-border-radius:6px;-webkit-border-radius:6px;border-radius:6px;border:1px solid #dcdcdc;display:inline-block;"
//                                 "cursor:pointer;color:#666666;font-family:Arial;font-size:15px;font-weight:bold;padding:6px 24px;text-decoration:none;"
//                                 "text-shadow:0px 1px 0px #ffffff;}"
//                                 "QPushButton:hover{background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #e9e9e9), color-stop(1, #f9f9f9));"
//                                 "background:-moz-linear-gradient(top, #e9e9e9 5%, #f9f9f9 100%);background:-webkit-linear-gradient(top, #e9e9e9 5%, #f9f9f9 100%);"
//                                 "background:-o-linear-gradient(top, #e9e9e9 5%, #f9f9f9 100%);background:-ms-linear-gradient(top, #e9e9e9 5%, #f9f9f9 100%);"
//                                 "background:linear-gradient(to bottom, #e9e9e9 5%, #f9f9f9 100%);filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#e9e9e9', endColorstr='#f9f9f9',GradientType=0);"
//                                 "background-color:#e9e9e9;}"
//                                 "QPushButton:pressed{position:relative;top:1px;}");


//    QFile qss(":/button/style/button.qss");
//    qss.open(QFile::ReadOnly);
//    certainButton->setStyleSheet(qss.readAll());
//    qss.close();
}



