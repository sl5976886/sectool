#include "sysmanagedialog.h"
#include "ui_sysmanagedialog.h"
#include "newuserdialog.h"
#include "newcomdialog.h"
#include "commonfunction.h"
#include "cleanlogdialog.h"
#include "noeditdelegate.h"
#include <QScrollBar>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QTableWidgetItem>
#include <QSqlQuery>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QToolButton>
#include <QCalendarWidget>

sysManageDialog::sysManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sysManageDialog)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    buttonStyle = QString("QPushButton{border:none;background-color:#484848;color:#DCDCDC;"
                          "border-style:none;border:1px solid #242424;border-radius:0px;}"
                          "QPushButton:hover{border:none;background-color:#383838;color:#DCDCDC;"
                          "border-style:none;border:1px solid #242424;border-radius:0px;}"
                          "QPushButton:pressed{border:none;background-color:#383838;color:#DCDCDC;"
                          "border-style:none;border:1px solid #242424;border-radius:0px;}");
    buttonStyle_2 = QString("QPushButton{border:none;background-color:#383838;color:#DCDCDC;}"
                            "border-style:none;border:1px solid #242424;border-radius:0px;"
                            "QPushButton:hover{border:none;background-color:#383838;color:#DCDCDC;"
                            "border-style:none;border:1px solid #242424;border-radius:0px;}"
                            "QPushButton:pressed{border:none;background-color:#383838;color:#DCDCDC;"
                            "border-style:none;border:1px solid #242424;border-radius:0px;}");

    buttonInit();
    ui->unitButton->setVisible(false);
    userTableViewInit();
    this->setWindowTitle(QString("系统管理"));
    this->setFixedSize(this->width(),this->height());
    QString action = QString("系统管理");
    QString state = QString("成功");
    operateLog(action,state);
    ui->startTimeEdit->setCalendarPopup(true);
    ui->endTimeEdit->setCalendarPopup(true);
    ui->startTimeEdit->calendarWidget()->findChild<QToolButton*>("qt_calendar_monthbutton")->setEnabled(false);
    ui->endTimeEdit->calendarWidget()->findChild<QToolButton*>("qt_calendar_monthbutton")->setEnabled(false);
    ui->endTimeEdit->setDateTime(QDateTime::currentDateTime());
    time_t time = ui->endTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time,strEndTime,128);

    sysSetShow();
}

sysManageDialog::~sysManageDialog()
{
    delete ui;
}

void sysManageDialog::userTableViewInit()
{
    userModel = new QSqlTableModel(this);
    userModel->setTable("register_user");
    userModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    userModel->select();
    int rowCount = userModel->rowCount();
    if(rowCount==0)
    {
        ui->noUserLabel->setVisible(true);
    }
    else
    {
        ui->noUserLabel->setVisible(false);
    }
    userModel->removeColumn(2);
    userModel->setHeaderData(0,Qt::Horizontal,QString("用户ID"));
    userModel->setHeaderData(1,Qt::Horizontal,QString("用户名"));
    userModel->setHeaderData(2,Qt::Horizontal,QString("调查单位"));
    userModel->setHeaderData(3,Qt::Horizontal,QString("警号"));
    userModel->setHeaderData(4,Qt::Horizontal,QString("真实姓名"));
    userModel->setHeaderData(5,Qt::Horizontal,QString("职务"));
    userModel->setHeaderData(6,Qt::Horizontal,QString("联系电话"));
    userModel->setHeaderData(7,Qt::Horizontal,QString("创建时间"));
    userModel->setHeaderData(8,Qt::Horizontal,QString("状态"));
    ui->userTableView->setModel(userModel);
    //设置无法编辑
    NoEditDelegate *mEditDele = new NoEditDelegate();
    ui->userTableView->setItemDelegateForColumn(2,mEditDele);

    ui->userTableView->hideColumn(9);
    ui->userTableView->hideColumn(10);
    ui->userTableView->hideColumn(11);
    ui->userTableView->setColumnWidth(0,50);
    ui->userTableView->setColumnWidth(7,120);
    ui->userTableView->verticalHeader()->setVisible(false);

    comModel = new QSqlTableModel(this);
    comModel->setTable("emergency_company");
    comModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    comModel->select();
    comModel->setHeaderData(0,Qt::Horizontal,QString("ID"));
    comModel->setHeaderData(1,Qt::Horizontal,QString("单位名称"));
    comModel->setHeaderData(2,Qt::Horizontal,QString("单位编码"));
    comModel->setHeaderData(3,Qt::Horizontal,QString("状态"));
    ui->comTableView->setModel(comModel);
    ui->comTableView->hideColumn(0);
    ui->comTableView->setColumnWidth(1,222);
    ui->comTableView->setColumnWidth(2,222);
    ui->comTableView->setColumnWidth(3,107);
    ui->comTableView->verticalHeader()->setVisible(false);
}

void sysManageDialog::buttonInit()
{
    ui->userButton->setStyleSheet(buttonStyle);
    ui->unitButton->setStyleSheet(buttonStyle);
    ui->logButton->setStyleSheet(buttonStyle);
    ui->configButton->setStyleSheet(buttonStyle);

//    ui->buttonLabel->setStyleSheet("background-color:rgb(240,247,253);"
//                                   "border: 1px solid rgb(222,227,232);");

//    ui->newUserButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->modifyButton_1->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->revokeButton_1->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->deleteButton_1->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->refreshButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->newComButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->modifyButton_2->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->revokeButton_2->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->refreshButton_2->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->inquiryButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->clearButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->exportButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->backupButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->restoreButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->cleanButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->saveConfigButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");

//    ui->restoreConfigButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 9pt Arial Rounded MT Bold ;border-radius:3px;border-style:outset;}"
//                                       "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                       "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");
}

//处置单位管理按钮
void sysManageDialog::on_unitButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->unitButton->setStyleSheet(buttonStyle_2);
    ui->userButton->setStyleSheet(buttonStyle);
    ui->logButton->setStyleSheet(buttonStyle);
    ui->configButton->setStyleSheet(buttonStyle);
}

//用户管理按钮
void sysManageDialog::on_userButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->userButton->setStyleSheet(buttonStyle_2);
    ui->unitButton->setStyleSheet(buttonStyle);
    ui->logButton->setStyleSheet(buttonStyle);
    ui->configButton->setStyleSheet(buttonStyle);
}

void sysManageDialog::on_logButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->logButton->setStyleSheet(buttonStyle_2);
    ui->userButton->setStyleSheet(buttonStyle);
    ui->unitButton->setStyleSheet(buttonStyle);
    ui->configButton->setStyleSheet(buttonStyle);
}

//配置管理按钮
void sysManageDialog::on_configButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->configButton->setStyleSheet(buttonStyle_2);
    ui->userButton->setStyleSheet(buttonStyle);
    ui->logButton->setStyleSheet(buttonStyle);
    ui->unitButton->setStyleSheet(buttonStyle);
}

//新增用户之后刷新界面
void sysManageDialog::refreshUserTable()
{
    refreshUser();
}

//新增用户按钮
void sysManageDialog::on_newUserButton_clicked()
{
   newUserDialog nud(this);
   connect(nud.certainButton,SIGNAL(clicked()),this,SLOT(refreshUserTable()));
   nud.show();
   nud.exec();
}

void sysManageDialog::refreshUser()
{
    userModel->setTable("register_user");
    userModel->select();
    int rowCount = userModel->rowCount();
    if(rowCount==0)
    {
        ui->noUserLabel->setVisible(true);
    }
    else
    {
        ui->noUserLabel->setVisible(false);
    }
    userModel->removeColumn(2);
    //userModel->removeColumn(0);
    userModel->setHeaderData(0,Qt::Horizontal,QString("用户ID"));
    userModel->setHeaderData(1,Qt::Horizontal,QString("用户名"));
    userModel->setHeaderData(2,Qt::Horizontal,QString("调查单位"));
    userModel->setHeaderData(3,Qt::Horizontal,QString("警号"));
    userModel->setHeaderData(4,Qt::Horizontal,QString("真实姓名"));
    userModel->setHeaderData(5,Qt::Horizontal,QString("职务"));
    userModel->setHeaderData(6,Qt::Horizontal,QString("联系电话"));
    userModel->setHeaderData(7,Qt::Horizontal,QString("创建时间"));
    userModel->setHeaderData(8,Qt::Horizontal,QString("状态"));
    ui->userTableView->verticalHeader()->setVisible(false);
}

//刷新数据
void sysManageDialog::on_refreshButton_clicked()
{
    userModel->setTable("register_user");
    userModel->select();
    int rowCount = userModel->rowCount();
    if(rowCount==0)
    {
        ui->noUserLabel->setVisible(true);
    }
    else
    {
        ui->noUserLabel->setVisible(false);
    }
    userModel->removeColumn(2);
    //userModel->removeColumn(0);
    userModel->setHeaderData(0,Qt::Horizontal,QString("用户ID"));
    userModel->setHeaderData(1,Qt::Horizontal,QString("用户名"));
    userModel->setHeaderData(2,Qt::Horizontal,QString("调查单位"));
    userModel->setHeaderData(3,Qt::Horizontal,QString("警号"));
    userModel->setHeaderData(4,Qt::Horizontal,QString("真实姓名"));
    userModel->setHeaderData(5,Qt::Horizontal,QString("职务"));
    userModel->setHeaderData(6,Qt::Horizontal,QString("联系电话"));
    userModel->setHeaderData(7,Qt::Horizontal,QString("创建时间"));
    userModel->setHeaderData(8,Qt::Horizontal,QString("状态"));
    ui->userTableView->verticalHeader()->setVisible(false);
}

//提交修改
void sysManageDialog::on_modifyButton_1_clicked()
{
    userModel->database().transaction();
    //qDebug()<<"transaction:"<<ok1;
    if (userModel->submitAll()) {   //返回值为true
       userModel->database().commit(); //返回值为true
       //qDebug()<<"commit:"<<ok;
    } else {
       userModel->database().rollback();
       QString strTitle = QString("警告");
       QString strMsg = QString("数据库出错");
       QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
       if(NULL!=WrrMsg->button(QMessageBox::Ok))
       {
           WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
       }
       WrrMsg->exec();
    }
}

//撤销修改
void sysManageDialog::on_revokeButton_1_clicked()
{
    userModel->revertAll();
}

//删除
void sysManageDialog::on_deleteButton_1_clicked()
{
    //获取选中的行
    int curRow = ui->userTableView->currentIndex().row();

    //删除该行
    userModel->removeRow(curRow);

    QString strTitle = QString("删除");
    QString strMsg = QString("你确定删除当前用户吗？");
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
        userModel->revertAll(); //如果不删除，则撤销
    }
    else
    {
        userModel->submitAll(); //否则提交，在数据库中删除该行
        refreshUser();
    }
}

//新增单位后刷新页面
void sysManageDialog::refreshComTable()
{
    comModel->setTable("emergency_company");
    comModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    comModel->select();
    comModel->setHeaderData(0,Qt::Horizontal,QString("ID"));
    comModel->setHeaderData(1,Qt::Horizontal,QString("单位名称"));
    comModel->setHeaderData(2,Qt::Horizontal,QString("单位编码"));
    comModel->setHeaderData(3,Qt::Horizontal,QString("状态"));
    ui->comTableView->hideColumn(0);
    ui->comTableView->setColumnWidth(1,222);
    ui->comTableView->setColumnWidth(2,222);
    ui->comTableView->setColumnWidth(3,105);
    ui->comTableView->verticalHeader()->setVisible(false);
}

//新增单位
void sysManageDialog::on_newComButton_clicked()
{
    newComDialog ncd(this);
    connect(ncd.certainButton,SIGNAL(clicked()),this,SLOT(refreshComTable()));
    ncd.show();
    ncd.exec();
}

//提交修改-处置单位
void sysManageDialog::on_modifyButton_2_clicked()
{
    comModel->database().transaction();
    //qDebug()<<"transaction:"<<ok1;
    if (comModel->submitAll()) {
       comModel->database().commit();
       //qDebug()<<"commit:"<<ok;
    } else {
       comModel->database().rollback();
       QString strTitle = QString("警告");
       QString strMsg = QString("数据库出错");
       QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
       if(NULL!=WrrMsg->button(QMessageBox::Ok))
       {
           WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
       }
       WrrMsg->exec();
    }
}

//撤销修改-处置单位
void sysManageDialog::on_revokeButton_2_clicked()
{
    comModel->revertAll();
}

//刷新数据-处置单位
void sysManageDialog::on_refreshButton_2_clicked()
{
    comModel->setTable("emergency_company");
    comModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    comModel->select();
    comModel->setHeaderData(0,Qt::Horizontal,QString("ID"));
    comModel->setHeaderData(1,Qt::Horizontal,QString("单位名称"));
    comModel->setHeaderData(2,Qt::Horizontal,QString("单位编码"));
    comModel->setHeaderData(3,Qt::Horizontal,QString("状态"));
    ui->comTableView->hideColumn(0);
    ui->comTableView->setColumnWidth(1,222);
    ui->comTableView->setColumnWidth(2,222);
    ui->comTableView->setColumnWidth(3,105);
    ui->comTableView->verticalHeader()->setVisible(false);
}

//查询操作日志
void sysManageDialog::on_inquiryButton_clicked()
{
    char strStartTime[128] = { 0 };
    time_t time = ui->startTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time,strStartTime,128);
    char nowEndTime[128] = { 0 };
    time_t time_2 = ui->endTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time_2,nowEndTime,128);
    if(time>time_2)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("结束时间需要晚于起始时间");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }

    logModel = new QSqlTableModel(this);
    logModel->setTable("operate_log");
    logModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QString user = ui->userQueryLineEdit->text();
    if(ui->userQueryLineEdit->text().length()>0)
    {
        logModel->setFilter(QString("user_name='%1' AND operate_time>'%2' AND operate_time<'%3'")
                            .arg(user).arg(strStartTime).arg(nowEndTime));
    }
    else
    {
        logModel->setFilter(QString("operate_time>'%1' AND operate_time<'%2'")
                            .arg(strStartTime).arg(nowEndTime));
    }

    logModel->select();
    logModel->setHeaderData(0,Qt::Horizontal,QString("用户名"));
    logModel->setHeaderData(1,Qt::Horizontal,QString("操作事件"));
    logModel->setHeaderData(2,Qt::Horizontal,QString("操作时间"));
    logModel->setHeaderData(3,Qt::Horizontal,QString("状态"));
    ui->logTableView->setModel(logModel);
    ui->logTableView->verticalHeader()->setVisible(false);
    ui->logTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->logTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->logTableView->setColumnWidth(0,150);
    ui->logTableView->setColumnWidth(1,150);
    ui->logTableView->setColumnWidth(2,180);
    ui->logTableView->setColumnWidth(3,139);
}

//清空日志
void sysManageDialog::on_clearButton_clicked()
{
    QSqlTableModel *cleanModel = new QSqlTableModel(this);
    cleanModel->setHeaderData(0,Qt::Horizontal,QString("用户名"));
    cleanModel->setHeaderData(1,Qt::Horizontal,QString("操作事件"));
    cleanModel->setHeaderData(2,Qt::Horizontal,QString("操作时间"));
    cleanModel->setHeaderData(3,Qt::Horizontal,QString("状态"));
    ui->logTableView->setModel(cleanModel);
    ui->logTableView->verticalHeader()->setVisible(false);
    ui->logTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->logTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->logTableView->setColumnWidth(0,150);
    ui->logTableView->setColumnWidth(1,150);
    ui->logTableView->setColumnWidth(2,180);
    ui->logTableView->setColumnWidth(3,139);
}

//导出日志
void sysManageDialog::on_exportButton_clicked()
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement operate_log = doc.createElement("operate");
    doc.appendChild(operate_log);
    char strStartTime[128] = { 0 };
    time_t time = ui->startTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time,strStartTime,128);
    char nowEndTime[128] = { 0 };
    time_t time_2 = ui->endTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time_2,nowEndTime,128);
    QSqlQuery query;
    QString strSql;
    QString strName,strAct,strTime,strState;
    QString user = ui->userQueryLineEdit->text();
    if(ui->userQueryLineEdit->text().length()>0)
    {
        strSql = QString("SELECT * FROM operate_log WHERE user_name='%1' AND operate_time>'%2' "
                                        "AND operate_time<'%3'").arg(user).arg(strStartTime).arg(nowEndTime);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
        }
    }
    else
    {
        strSql = QString("SELECT * FROM operate_log WHERE operate_time>'%1' AND operate_time<'%2'")
                .arg(strStartTime).arg(nowEndTime);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
        }
    }
    while (query.next())
    {
        strName = query.value(0).toString();
        strAct = query.value(1).toString();
        strTime = query.value(2).toString();
        strState = query.value(3).toString();
        QDomElement item = doc.createElement("item");
        operate_log.appendChild(item);

        QDomElement user_name = doc.createElement("user_name");
        QDomText text_1 = doc.createTextNode(strName);
        user_name.appendChild(text_1);
        item.appendChild(user_name);

        QDomElement action = doc.createElement("user_name");
        QDomText text_2 = doc.createTextNode(strAct);
        action.appendChild(text_2);
        item.appendChild(action);

        QDomElement time = doc.createElement("time");
        QDomText text_3 = doc.createTextNode(strTime);
        time.appendChild(text_3);
        item.appendChild(time);

        QDomElement state = doc.createElement("state");
        QDomText text_4 = doc.createTextNode(strState);
        state.appendChild(text_4);
        item.appendChild(state);
    }
    QString saveFileName = QFileDialog::getSaveFileName(this,QString("导出日志"),".","(*.olog)");
    if(saveFileName.isEmpty())
    {
        return;
    }
    QFile file(saveFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        qWarning()<<"open file fail ";
        QString strTitle = QString("警告");
        QString strMsg = QString("打开文件出错");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();
}

void sysManageDialog::on_backupButton_clicked()
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement operate_log = doc.createElement("operate");
    doc.appendChild(operate_log);
    char strStartTime[128] = { 0 };
    time_t time = ui->startTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time,strStartTime,128);
    char nowEndTime[128] = { 0 };
    time_t time_2 = ui->endTimeEdit->dateTime().toTime_t();
    GetAsciiTime(time_2,nowEndTime,128);
    QSqlQuery query;
    QString strSql;
    QString strName,strAct,strTime,strState;
    QString user = ui->userQueryLineEdit->text();
    if(ui->userQueryLineEdit->text().length()>0)
    {
        strSql = QString("SELECT * FROM operate_log WHERE user_name='%1' AND operate_time>'%2' "
                                        "AND operate_time<'%3'").arg(user).arg(strStartTime).arg(nowEndTime);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
        }
    }
    else
    {
        strSql = QString("SELECT * FROM operate_log WHERE operate_time>'%1' AND operate_time<'%2'")
                .arg(strStartTime).arg(nowEndTime);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
        }
    }
    while (query.next())
    {
        strName = query.value(0).toString();
        strAct = query.value(1).toString();
        strTime = query.value(2).toString();
        strState = query.value(3).toString();
        QDomElement item = doc.createElement("item");
        operate_log.appendChild(item);

        QDomElement user_name = doc.createElement("user_name");
        QDomText text_1 = doc.createTextNode(strName);
        user_name.appendChild(text_1);
        item.appendChild(user_name);

        QDomElement action = doc.createElement("user_name");
        QDomText text_2 = doc.createTextNode(strAct);
        action.appendChild(text_2);
        item.appendChild(action);

        QDomElement time = doc.createElement("time");
        QDomText text_3 = doc.createTextNode(strTime);
        time.appendChild(text_3);
        item.appendChild(time);

        QDomElement state = doc.createElement("state");
        QDomText text_4 = doc.createTextNode(strState);
        state.appendChild(text_4);
        item.appendChild(state);
    }
    QString saveFileName = QFileDialog::getSaveFileName(this,QString("导出日志"),".","(*.bck)");
    if(saveFileName.isEmpty())
    {
        return;
    }
    QFile file(saveFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        qWarning()<<"open file fail ";
        QString strTitle = QString("警告");
        QString strMsg = QString("打开文件出错");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();
}

//清理日志
void sysManageDialog::on_cleanButton_clicked()
{
    cleanLogDialog cld(this);
    cld.show();
    cld.exec();
}

//安全设置显示
void sysManageDialog::sysSetShow()
{
    QSqlQuery query;
    QString strSql;
    strSql = QString("SELECT * FROM sys_set WHERE user_name = '%1'").arg(g_user_name);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err: "<<query.lastError();
        QString strTitle = QString("警告");
        QString strMsg = QString("数据库出错");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }
    int count = query.size();
    if(count==0)
    {
    }
    else if(count==1)
    {
        if(query.next())
        {
            int mWrong = query.value(1).toInt();
            int mLock = query.value(2).toInt();
            int mMax = query.value(3).toInt();
            int mAccount = query.value(4).toInt();
            QString strWrong = QString::number(mWrong,10);
            QString strLock = QString::number(mLock,10);
            QString strMax = QString::number(mMax,10);
            QString strAccount = QString::number(mAccount,10);
            ui->wrongLineEdit->setText(strWrong);
            ui->lockLineEdit->setText(strLock);
            ui->maxLineEdit->setText(strMax);
            ui->accountLineEdit->setText(strAccount);
        }
    }
}

//保存安全设置
void sysManageDialog::on_saveConfigButton_clicked()
{
    int mWrong = ui->wrongLineEdit->text().toInt();
    int mLock = ui->lockLineEdit->text().toInt();
    int mMax = ui->maxLineEdit->text().toInt();
    int mAccount = ui->accountLineEdit->text().toInt();
    QSqlQuery query;
    QString strSql;
    strSql = QString("SELECT * FROM sys_set WHERE user_name = '%1'").arg(g_user_name);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err: "<<query.lastError();
        QString strTitle = QString("警告");
        QString strMsg = QString("数据库出错");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }
    int count = query.size();
    if(count==0)
    {
        strSql = QString("INSERT INTO sys_set(user_name,wrong_num,lock_time,max_num,account_lock_time)"
                                        "VALUES('%1',%2,%3,%4,%5)").arg(g_user_name).arg(mWrong).arg(mLock)
                .arg(mMax).arg(mAccount);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql err: "<<query.lastError();
            QString strTitle = QString("警告");
            QString strMsg = QString("数据库出错");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
        }
    }
    else if(count==1)
    {
        strSql = QString("UPDATE sys_set SET wrong_num=%1,lock_time=%2,max_num=%3,account_lock_time=%4"
                                        " WHERE user_name='%5'").arg(mWrong).arg(mLock).arg(mMax).arg(mAccount).arg(g_user_name);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql err: "<<query.lastError()<<strSql.toStdString().c_str();
            QString strTitle = QString("警告");
            QString strMsg = QString("数据库出错");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
        }
    }
    else
    {
        qWarning()<<"query sys_log num error!";
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
    QString strTitle = QString("成功");
    QString strMsg = QString("设置已保存");
    QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
    if(NULL!=WrrMsg->button(QMessageBox::Ok))
    {
        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
    }
    WrrMsg->exec();
}


//恢复出厂设置
void sysManageDialog::on_restoreConfigButton_clicked()
{
    int mWrong = 3;
    int mLock = 600;
    int mMax = 10000;
    int mAccount = 300;
    QSqlQuery query;
    QString strSql;
    strSql = QString("SELECT * FROM sys_set WHERE user_name = '%1'").arg(g_user_name);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err: "<<query.lastError();
        QString strTitle = QString("警告");
        QString strMsg = QString("数据库出错");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }
    int count = query.size();
    if(count==0)
    {
        strSql = QString("INSERT INTO sys_set(user_name,wrong_num,lock_time,max_num,account_lock_time)"
                                        "VALUES('%1',%2,%3,%4,%5)").arg(g_user_name).arg(mWrong).arg(mLock)
                .arg(mMax).arg(mAccount);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql err: "<<query.lastError();
            QString strTitle = QString("警告");
            QString strMsg = QString("数据库出错");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
        }
    }
    else if(count==1)
    {
        strSql = QString("UPDATE sys_set SET wrong_num=%1,lock_time=%2,max_num=%3,account_lock_time=%4"
                                        " WHERE user_name='%5'").arg(mWrong).arg(mLock).arg(mMax).arg(mAccount).arg(g_user_name);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql err: "<<query.lastError()<<strSql.toStdString().c_str();
            QString strTitle = QString("警告");
            QString strMsg = QString("数据库出错");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
        }
    }
    else
    {
        qWarning()<<"query sys_log num error!";
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
    QString strTitle = QString("成功");
    QString strMsg = QString("已恢复出厂设置");
    QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
    if(NULL!=WrrMsg->button(QMessageBox::Ok))
    {
        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
    }
    WrrMsg->exec();
    ui->wrongLineEdit->setText("3");
    ui->lockLineEdit->setText("600");
    ui->maxLineEdit->setText("10000");
    ui->accountLineEdit->setText("300");
}
