#include "newtaskdialog.h"
#include "ui_newtaskdialog.h"
#include "commonfunction.h"
#include "disposedialog.h"
#include "taskmanagedialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <time.h>
#include <QDomDocument>
#include <QCalendarWidget>
#include <QToolButton>

newTaskDialog::newTaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newTaskDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("新建任务"));
    //this->setWindowFlags(Qt::WindowTitleHint);
    this->setFixedSize(this->width(),this->height());
    QObject::connect(ui->pathButton,SIGNAL(clicked()),this,SLOT(saveReportSlot()));

    QDateTime currentTime = QDateTime::currentDateTime();
    ui->dateEdit->setDateTime(currentTime);
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->dateEdit->calendarWidget()->findChild<QToolButton*>("qt_calendar_monthbutton")->setEnabled(false);

    ui->label_17->setVisible(false);
    ui->savePlaceLineEdit->setVisible(false);
    ui->pathButton->setVisible(false);

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
    ui->disposeComComboBox->setFixedHeight(20);
    while(query.next())
    {
        QString name = query.value(0).toString();
        QPixmap pixmap(1, ui->disposeComComboBox->height());
        pixmap.fill(Qt::transparent);
        QIcon icon(pixmap);
        ui->disposeComComboBox->setIconSize(QSize(1,ui->disposeComComboBox->height()));
        ui->disposeComComboBox->addItem(icon,name);
    }
    mmw = NULL;
    tmm = NULL;
}

newTaskDialog::~newTaskDialog()
{
    delete ui;
}

void newTaskDialog::hookMainDialog(mainDialog *w)
{
    mmw = w;
}

void newTaskDialog::hookTaskManage(taskManageDialog *w)
{
    tmm = w;
}

void newTaskDialog::saveReportSlot()
{
    saveFileName = QFileDialog::getSaveFileName(this,QString("保存文件"),QDir::currentPath(),"(*.pro)");
    if(saveFileName.isEmpty())
    {
        //QMessageBox::information(this,QString("出错"),QString("请选择文件"));
        return;
    }
    else
    {
        ui->savePlaceLineEdit->setText(saveFileName);
    }
}


void newTaskDialog::on_pushButton_clicked()
{
    QString strOrg = ui->disposeComComboBox->currentText();
    if(ui->comLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请填写事发单位");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->comLineEdit->setFocus();
        return;
    }
    else if(ui->eventPlaceLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请填写事发地点");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->eventPlaceLineEdit->setFocus();
        return;
    }
    else if(ui->eventNameLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请填写事件名称");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->eventNameLineEdit->setFocus();
        return;
    }
    else if(strOrg.isEmpty())
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择处置单位");
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
        char strLastTime[128];
        time_t time = ui->dateEdit->dateTime().toTime_t();
        GetAsciiTime(time,strLastTime,128);
        QSqlQuery query;

        QString strSql = QString("UPDATE main_task SET state='关闭' WHERE state<>'关闭'");
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql err:"<<query.lastError();
        }

        strSql = QString("INSERT INTO "
                         "main_task(event_company,company_address,principal,phone,office_phone,email,"
                                                "system_name,system_level,event_source,event_name,event_time,event_place,"
                                                "event_kind,event_level,dispose_company,event_discribe,save_place,state,user_name)"
                                                "VALUES(\'%1\',\'%2\',\'%3\',\'%4\',\'%5\',\'%6\',\'%7\','%8',\'%9\',"
                                                "\'%10\',\'%11\',\'%12\',\'%13\',\'%14\',\'%15\',\'%16\',\'%17\','开启','%18')")
                                                 .arg(ui->comLineEdit->text())
                                                 .arg(ui->comAddrLineEdit->text())
                                                 .arg(ui->principalLineEdit->text())
                                                 .arg(ui->phoneLineEdi->text())
                                                 .arg(ui->officePhoneLineEdit->text())
                                                 .arg(ui->emailLineEdit->text())
                                                 .arg(ui->systemNameLineEdit->text())
                                                 .arg(ui->sysLevelComboBox->currentText())
                                                 .arg(ui->eventSourceComboBox->currentText())
                                                 .arg(ui->eventNameLineEdit->text())
                                                 .arg(strLastTime)
                                                 .arg(ui->eventPlaceLineEdit->text())
                                                 .arg(ui->eventKindComboBox->currentText())
                                                 .arg(ui->eventLevelComboBox->currentText())
                                                 .arg(ui->disposeComComboBox->currentText())
                                                 .arg(ui->discribeTextEdit->toPlainText())
                                                 .arg(ui->savePlaceLineEdit->text())
                                                 .arg(g_user_name);
        //qDebug()<<"sql is:"<<strSql;
        ok = query.exec(strSql);
        if(!ok)
        {
            //qDebug()<<query.lastError();
            qWarning()<<"sql err:"<<query.lastError();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错，请检查数据库连接后重试");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
        strSql = QString("SELECT MAX(id) FROM main_task");
        ok = query.exec(strSql);
        if(!ok)
        {
            //error log
            qWarning()<<"sql err:"<<query.lastError();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错，请检查数据库连接后重试");
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
            g_task_id = query.value(0).toInt();
            //qDebug()<<"task id is:"<<g_task_id;
        }
        strSql = QString("INSERT INTO safe_record_form(id,company_name)VALUES('%1','%2')")
                .arg(g_task_id).arg(ui->comLineEdit->text());
        ok = query.exec(strSql);
        if(!ok)
        {
            //error log
            qWarning()<<"sql err:"<<query.lastError();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错，请检查数据库连接后重试");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
//        strSql = QString("INSERT INTO web_record_form(id)VALUES('%1')").arg(g_task_id);
//        ok = query.exec(strSql);
//        if(!ok)
//        {
//            //error log
//        }

        QString action = QString("新建任务");
        QString state = QString("成功");
        operateLog(action,state);
        QString strTitle = QString("提示");
        QString strMsg = QString("新建任务成功");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();


        //注：以下为任务另存为的代码，目前版本已屏蔽
//        QString eventCompany,comAddress,principal,strPhone,officePhone,strEmail,sysName,sysLevel,eventSource,
//                eventName,eventTime,eventPlace,eventKind,eventLevel,disCompany,eventDiscribe,savePlace;

//        if(ui->savePlaceLineEdit->text().length()>0)
//        {
//            eventCompany = ui->comLineEdit->text();
//            comAddress = ui->comAddrLineEdit->text();
//            principal = ui->principalLineEdit->text();
//            strPhone = ui->phoneLineEdi->text();
//            officePhone = ui->officePhoneLineEdit->text();
//            strEmail = ui->emailLineEdit->text();
//            sysName = ui->systemNameLineEdit->text();
//            sysLevel = ui->sysLevelComboBox->currentText();
//            eventSource = ui->eventSourceComboBox->currentText();
//            eventName = ui->eventNameLineEdit->text();
//            eventTime = strLastTime;
//            eventPlace = ui->eventPlaceLineEdit->text();
//            eventKind = ui->eventKindComboBox->currentText();
//            eventLevel = ui->eventLevelComboBox->currentText();
//            disCompany = ui->disposeComComboBox->currentText();
//            eventDiscribe = ui->discribeTextEdit->toPlainText();
//            savePlace = ui->savePlaceLineEdit->text();

//            QDomDocument doc;
//            QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
//            doc.appendChild(instruction);
//            QDomElement task = doc.createElement("Task");
//            doc.appendChild(task);
//            QDomElement taskInfo = doc.createElement("task_info");
//            taskInfo.setAttribute("taskId",g_task_id);
//            taskInfo.setAttribute("state",QString("正常"));
//            task.appendChild(taskInfo);
//            QDomElement comInfo = doc.createElement("com_info");
//            task.appendChild(comInfo);

//            QDomElement comName = doc.createElement("com_name");
//            QDomText text_1 = doc.createTextNode(eventCompany);
//            comName.appendChild(text_1);
//            comInfo.appendChild(comName);

//            QDomElement comAddr = doc.createElement("com_addr");
//            QDomText text_2 = doc.createTextNode(comAddress);
//            comAddr.appendChild(text_2);
//            comInfo.appendChild(comAddr);

//            QDomElement princi = doc.createElement("principal");
//            QDomText text_3 = doc.createTextNode(principal);
//            princi.appendChild(text_3);
//            comInfo.appendChild(princi);

//            QDomElement phone = doc.createElement("phone");
//            QDomText text_4 = doc.createTextNode(strPhone);
//            phone.appendChild(text_4);
//            comInfo.appendChild(phone);

//            QDomElement office = doc.createElement("officePhone");
//            QDomText text_5 = doc.createTextNode(officePhone);
//            office.appendChild(text_5);
//            comInfo.appendChild(office);

//            QDomElement email = doc.createElement("email");
//            QDomText text_6 = doc.createTextNode(strEmail);
//            email.appendChild(text_6);
//            comInfo.appendChild(email);

//            QDomElement systemName = doc.createElement("systemName");
//            QDomText text_7 = doc.createTextNode(sysName);
//            systemName.appendChild(text_7);
//            comInfo.appendChild(systemName);

//            QDomElement systemLevel = doc.createElement("systemLevel");
//            QDomText text_8 = doc.createTextNode(sysLevel);
//            systemLevel.appendChild(text_8);
//            comInfo.appendChild(systemLevel);

//            QDomElement eventS = doc.createElement("eventSource");
//            QDomText text_9 = doc.createTextNode(eventSource);
//            eventS.appendChild(text_9);
//            comInfo.appendChild(eventS);

//            QDomElement dispose_info = doc.createElement("dispose_info");
//            task.appendChild(dispose_info);

//            QDomElement eventN = doc.createElement("eventName");
//            QDomText text_10 = doc.createTextNode(eventName);
//            eventN.appendChild(text_10);
//            dispose_info.appendChild(eventN);

//            QDomElement eventT = doc.createElement("eventTime");
//            QDomText text_11 = doc.createTextNode(eventTime);
//            eventT.appendChild(text_11);
//            dispose_info.appendChild(eventT);

//            QDomElement eventP = doc.createElement("eventPlace");
//            QDomText text_12 = doc.createTextNode(eventPlace);
//            eventP.appendChild(text_12);
//            dispose_info.appendChild(eventP);

//            QDomElement eventK = doc.createElement("eventKind");
//            QDomText text_13 = doc.createTextNode(eventKind);
//            eventK.appendChild(text_13);
//            dispose_info.appendChild(eventK);

//            QDomElement eventL = doc.createElement("eventLevel");
//            QDomText text_14 = doc.createTextNode(eventLevel);
//            eventL.appendChild(text_14);
//            dispose_info.appendChild(eventL);

//            QDomElement disCom = doc.createElement("disCompany");
//            QDomText text_15 = doc.createTextNode(disCompany);
//            disCom.appendChild(text_15);
//            dispose_info.appendChild(disCom);

//            QDomElement eventDis = doc.createElement("eventDiscribe");
//            QDomText text_16 = doc.createTextNode(eventDiscribe);
//            eventDis.appendChild(text_16);
//            dispose_info.appendChild(eventDis);

//            QDomElement svPlace = doc.createElement("savePlace");
//            QDomText text_17 = doc.createTextNode(savePlace);
//            svPlace.appendChild(text_17);
//            dispose_info.appendChild(svPlace);

//            QFile file(saveFileName);
//            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
//            {
//                qWarning()<<"open file fail ";
//                return;
//            }
//            QTextStream out(&file);
//            out.setCodec("UTF-8");
//            doc.save(out,4,QDomNode::EncodingFromTextStream);
//            file.close();
//        }

        if(mmw)
        {
            connect(this,SIGNAL(openTask()),mmw,SLOT(openTaskShow_slot()));
            emit openTask();
        }
        else if(tmm)
        {
            connect(this,SIGNAL(openTask()),tmm,SLOT(task_fresh_slot()));
            emit openTask();
        }
        this->close();
    }
}


void newTaskDialog::on_pushButton_2_clicked()
{
    this->close();
}


