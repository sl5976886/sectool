#include "addhostdialog.h"
#include "ui_addhostdialog.h"
#include "commonfunction.h"
#include "disposedialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QLayout>

addHostDialog::addHostDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addHostDialog)
{
    ui->setupUi(this);
    QString action = QString("添加资产");
    QString state = QString("成功");
    operateLog(action,state);
    this->setWindowTitle(QString("添加资产"));
    this->setFixedSize(this->width(),this->height());
    qInstallMessageHandler(outputMessage);
    radioButtonInit();

    ui->ipLineEdit->setStyleSheet("border:1px groove gray;"
                                    "border-radius:0px");
    ui->nameLineEdit->setStyleSheet("border:1px groove gray;"
                                    "border-radius:0px");

    certainButton = new QPushButton(QString("确定"),this);
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);

    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(certain_click()));
}

addHostDialog::~addHostDialog()
{
    delete ui;
}

void addHostDialog::radioButtonInit()
{
    kindGroup = new QButtonGroup(this);
    kindGroup->setExclusive(true);
    kindGroup->addButton(ui->kindRadioButton_1);
    kindGroup->addButton(ui->kindRadioButton_2);
    kindGroup->addButton(ui->kindRadioButton_3);
    kindGroup->setId(ui->kindRadioButton_1,1);
    kindGroup->setId(ui->kindRadioButton_2,2);
    kindGroup->setId(ui->kindRadioButton_3,3);

    sysGroup = new QButtonGroup(this);
    sysGroup->setExclusive(true);
    sysGroup->addButton(ui->sysRadioButton_1);
    sysGroup->addButton(ui->sysRadioButton_2);
    sysGroup->setId(ui->sysRadioButton_1,1);
    sysGroup->setId(ui->sysRadioButton_2,2);

    dataGroup = new QButtonGroup(this);
    dataGroup->setExclusive(false);
    dataGroup->addButton(ui->dateCheckBox_1);
    dataGroup->addButton(ui->dateCheckBox_2);
    dataGroup->addButton(ui->dateCheckBox_3);
    dataGroup->addButton(ui->dateCheckBox_4);
    dataGroup->addButton(ui->dateCheckBox_5);

    middleGroup = new QButtonGroup(this);
    middleGroup->setExclusive(false);
    middleGroup->addButton(ui->middleCheckBox_1);
    middleGroup->addButton(ui->middleCheckBox_2);
    middleGroup->addButton(ui->middleCheckBox_3);
    middleGroup->addButton(ui->middleCheckBox_4);
    middleGroup->addButton(ui->middleCheckBox_5);
    middleGroup->addButton(ui->middleCheckBox_6);
    middleGroup->addButton(ui->middleCheckBox_7);

    toolGroup = new QButtonGroup(this);
    toolGroup->setExclusive(false);
    toolGroup->addButton(ui->toolCheckBox_1);
    toolGroup->addButton(ui->toolCheckBox_2);
    toolGroup->addButton(ui->toolCheckBox_3);
    toolGroup->addButton(ui->toolCheckBox_4);
    toolGroup->addButton(ui->toolCheckBox_5);
    toolGroup->addButton(ui->toolCheckBox_6);
    toolGroup->addButton(ui->toolCheckBox_7);
    toolGroup->addButton(ui->toolCheckBox_8);
}

void addHostDialog::certain_click()
{
    if(g_task_id==0)
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("没有打开的任务，请先打开一个任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        return;
    }
    if(ui->nameLineEdit->text().length()==0)
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请填写资产名称！");
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
    if(ui->ipLineEdit->text().length()==0)
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请填写IP\\URL");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        ui->ipLineEdit->setFocus();
        return;
    }
    int checkId = toolGroup->checkedId();
    if(checkId==-1)
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请至少选择一个扫描工具");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        return;
    }
    QString kind;
    quint16 kind_id = kindGroup->checkedId();
    if(kind_id==1)
    {
        kind = QString("主机/服务器");
    }
    else if(kind_id==2)
    {
        kind = QString("存储设备(日志/备份)");
    }
    else if(kind_id==3)
    {
        kind = QString("边界设备");
    }
    else
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请选择资产类型！");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        return;
    }
    QString system;
    quint16 sys_id = sysGroup->checkedId();
    if(sys_id==1)
    {
        system = QString("Windows");
    }
    else if(sys_id==2)
    {
        system = QString("Linux");
    }
    else
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请选择操作系统");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        return;
    }
    QString database = QString("");
    if(ui->dateCheckBox_1->isChecked())
    {
        database+=QString("Sql Server,");
    }
    if(ui->dateCheckBox_2->isChecked())
    {
        database+=QString("Mysql,");
    }
    if(ui->dateCheckBox_3->isChecked())
    {
        database+=QString("Oracle,");
    }
    if(ui->dateCheckBox_4->isChecked())
    {
        database+=QString("Access,");
    }
    if(ui->dateCheckBox_5->isChecked())
    {
        database+=QString("其他,");
    }
//    int n = database.size();
//    database.chop(n);
    QString middle = QString("");
    if(ui->middleCheckBox_1->isChecked())
    {
        middle+=QString("Tomcat,");
    }
    if(ui->middleCheckBox_2->isChecked())
    {
        middle+=QString("Apache,");
    }
    if(ui->middleCheckBox_3->isChecked())
    {
        middle+=QString("IIS,");
    }
    if(ui->middleCheckBox_4->isChecked())
    {
        middle+=QString("Weblogic,");
    }
    if(ui->middleCheckBox_5->isChecked())
    {
        middle+=QString("Websphere,");
    }
    if(ui->middleCheckBox_6->isChecked())
    {
        middle+=QString("Jboss,");
    }
    if(ui->middleCheckBox_7->isChecked())
    {
        middle+=QString("其他,");
    }
//    n = middle.size();
//    middle.chop(n);
    QString sql1;
    QSqlQuery query;
    QString toolName;
    QString sql = QString("SELECT host_name FROM check_task WHERE id = %1").arg(g_task_id);
    bool ok =  query.exec(sql);
    if(!ok)
    {
        qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql;
    }
    if(query.next())
    {
        QString hostName = query.value(0).toString();
        if(hostName==ui->nameLineEdit->text())
        {
            QString strTitle = QString("错误");
            QString strMsg = QString("已存在相同名称的资产");
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
    sql = QString("INSERT INTO host_info(id,host_name,ip_address,kind,system) VALUES(%1,'%2','%3','%4','%5')")
            .arg(g_task_id).arg(ui->nameLineEdit->text()).arg(ui->ipLineEdit->text()).arg(kind).arg(system);
    ok=query.exec(sql);
    if(!ok)
    {
        qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql;
    }
    if(ui->toolCheckBox_1->isChecked())
    {
        toolName = QString("Windows主机配置检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_2->isChecked())
    {
        toolName = QString("病毒检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_3->isChecked())
    {
        toolName = QString("木马检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_4->isChecked())
    {
        toolName = QString("网站恶意代码检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_5->isChecked())
    {
        toolName = QString("系统漏洞检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_6->isChecked())
    {
        toolName = QString("网站安全检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_7->isChecked())
    {
        toolName = QString("数据库安全检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_8->isChecked())
    {
        toolName = QString("Linux主机配置检查工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    if(ui->toolCheckBox_9->isChecked())
    {
        toolName = QString("SQL注入验证工具");
        sql1 = QString("INSERT INTO check_task (id,host_name,ip_address,host_kind,datebase,middleware,"
                                      "operate_system,run_tool) VALUES (%1,'%2','%3','%4','%5','%6','%7','%8')")
                                      .arg(g_task_id).arg(ui->nameLineEdit->text())
                                      .arg(ui->ipLineEdit->text()).arg(kind).arg(database).arg(middle).arg(system).arg(toolName);
        ok=query.exec(sql1);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<"  sql:"<<sql1;
        }
    }
    QString strTitle = QString("成功");
    QString strMsg = QString("已添加资产");
    QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
    if(NULL!=WrrMsg->button(QMessageBox::Ok))

    {

        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

    }
    WrrMsg->exec();
    qDebug()<<"add host success,id:"<<g_task_id<<" name:"<<ui->nameLineEdit->text().toStdString().c_str();
    this->close();
}











