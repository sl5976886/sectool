#include "maindialog.h"
#include "ui_maindialog.h"
#include "disposedialog.h"
#include "sysmanagedialog.h"
#include "commonfunction.h"
#include "toolsetdialog.h"
#include "worddialog.h"
#include "disposedialog2.h"
#include "taskmanagedialog.h"
#include "modifydialog.h"
#include "modifypersondialog.h"
#include "newuserdialog.h"
#include "newtaskdialog.h"
#include "addhostdialog.h"
#include "adddispersondialog.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QToolButton>
#include <QDomElement>
#include <QFile>
#include <QAxObject>
#include <QAxWidget>
#include <QDirIterator>
#include <QLibrary>

//标题栏的长度
int pos_min_x = 0;
int pos_max_x = 903 - 50;  //为最小化和关闭按钮留空间
int pos_min_y = 0;
int pos_max_y = 30;

mainDialog::mainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->width(),this->height());
    this->setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮
//    this->setWindowTitle(QString("国瑞信安网络安全事件应急处置工具箱系统软件V2.0"));
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉窗口标题栏
    initTitleBar();

    ui->otherLabel->setAlignment(Qt::AlignCenter);

    buttonInit();
    ui->saveButton->setVisible(false);
    initTaskState();   //将所有任务状态置为未完成

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(connect_db_slot()));
    timer->start(1000*3600*6);

    ui->mainStackedWidget->setCurrentIndex(0);
}

void mainDialog::initTaskState()
{
    QSqlQuery query;
    QString strSql = QString("UPDATE main_task SET state='关闭' WHERE state<>'关闭'");
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err:"<<query.lastError();
    }
}

void mainDialog::initTitleBar()
{
    int width = this->width();//获取界面的宽度
    QToolButton *minButton = new QToolButton(this);   //最小按钮
    QToolButton *closeButton = new QToolButton(this);  //关闭按钮
    QLabel *titleLabel = new QLabel(this);   //标题
    QLabel *iconLabel = new QLabel(this);  //图标
    QObject::connect(minButton, SIGNAL(clicked()), this, SLOT(on_actionMinimize_triggered()));
    QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(on_actionClose_triggered()));

    //获取最小化、关闭按钮图标
    QPixmap minPix  = QPixmap(":/button/subtract.png");
    QPixmap closePix = QPixmap(":/button/close.png");
    QPixmap iconPix = QPixmap(":/button/shield.png");

    //设置最小化、关闭按钮图标
    minButton->setIcon(minPix);
    closeButton->setIcon(closePix);

    //设置最小化、关闭按钮在界面的位置
    minButton->setGeometry(width-50,0,25,25);
    closeButton->setGeometry(width-25,0,25,25);
    titleLabel->setGeometry(30,0,300,30);
    iconLabel->setGeometry(5,5,20,20);

    //设置鼠标移至按钮上的提示信息
    minButton->setToolTip(QString("最小化"));
    closeButton->setToolTip(QString("关闭"));
    titleLabel->setText(QString("国瑞应急处理工具箱软件"));
    iconLabel->setScaledContents(true);
    iconLabel->setPixmap(iconPix);

    //设置最小化、关闭等按钮的样式
    QString m_style = QString("QToolButton{border:none;background-color:#484848;border-radius:0px;}"
                              "QToolButton:hover{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252);}"
                              "QToolButton:pressed{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #484848,stop:1 #383838);}");
    minButton->setStyleSheet(m_style);
    closeButton->setStyleSheet(m_style);
    titleLabel->setStyleSheet(QString("background-color:transparent;font:10pt 微软雅黑"));
}

//自己实现的窗口拖动操作
void mainDialog::mousePressEvent(QMouseEvent *event)
{
        //当鼠标单击窗体准备拖动时，初始化鼠标在窗体中的相对位置
        mousePosition = event->pos();
        //只对标题栏范围内的鼠标事件进行处理
        if (mousePosition.x()<=pos_min_x)
            return;
        if ( mousePosition.x()>=pos_max_x)
            return;
        if (mousePosition.y()<=pos_min_y )
            return;
        if (mousePosition.y()>=pos_max_y)
            return;
        isMousePressed = true;
}

void mainDialog::mouseMoveEvent(QMouseEvent *event)
{
    if ( isMousePressed==true )
    {
        QPoint movePot = event->globalPos() - mousePosition;
        //move是移动的位置是相对于全局而言(即屏幕)
        move(movePot);
    }
}
void mainDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMousePressed=false;
}

void mainDialog::on_actionMinimize_triggered()
{
//    ui->stackedWidget->setCurrentIndex(0);
//    QWidget::hide();

    if( windowState() != Qt::WindowMinimized ){
        setWindowState( Qt::WindowMinimized );
    }

}
void mainDialog::on_actionClose_triggered()
{
    //系统自定义的窗口关闭函数
    close();
}

void mainDialog::buttonInit()
{
    QString style = QString("QPushButton{border-style:none;border:1px solid #242424;color:#DCDCDC;"
                            "padding:5px;min-height:15px;border-radius:5px;"
                            "background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #484848,stop:1 #383838);"
                            "font:87 17pt Arial Rounded MT Bold ;}"
                            "QPushButton:hover{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #646464,stop:1 #525252);"
                            "font:87 17pt Arial Rounded MT Bold ;}"
                            "QPushButton:pressed{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #484848,stop:1 #383838);"
                            "font:87 17pt Arial Rounded MT Bold ;}");
    ui->newTaskButton->setStyleSheet(style);
    ui->openTaskButton->setStyleSheet(style);

    QString style_2 = QString("QPushButton{border:none;background-color:#444444;}"
                              "QPushButton:hover{border:none;background-color:#444444;}"
                              "QPushButton:pressed{border:none;background-color:#444444;}");
    ui->pushButton->setStyleSheet(style_2);
    ui->pushButton_2->setStyleSheet(style_2);
    ui->treatButton->setStyleSheet(style_2);
    ui->wordButton->setStyleSheet(style_2);
    ui->userButton->setStyleSheet(style_2);
    ui->webscanButton->setStyleSheet(style_2);
    ui->webscanButton_2->setStyleSheet(style_2);
}

mainDialog::~mainDialog()
{
    QSqlQuery query;
    QString strSql = QString("UPDATE main_task SET state = '关闭' WHERE state = '开启'");
    query.exec(strSql);
    reportThread.wait();
    reportThread.stop();
    //qDebug()<<"mainDialog uninit";
    delete ui;
}

void mainDialog::openWordReport()
{
}

void mainDialog::connect_db_slot()
{
    QString strSql = QString("SELECT 'x'");
    QSqlQuery query;
    query.exec(strSql);
}

//新建任务按钮
void mainDialog::on_newTaskButton_clicked()
{
    newTaskDialog nw(this);
    nw.setModal(true);
    nw.hookMainDialog(this);
    nw.show();
    nw.exec();
}

//任务管理按钮
void mainDialog::on_pushButton_2_clicked()
{
    if(g_task_id!=0)
    {
        ui->mainStackedWidget->setCurrentIndex(1);
    }
    else
    {
        taskManageDialog tmw(this);
        tmw.setModal(true);
        tmw.hookMainDialog(this);
        tmw.show();
        tmw.exec();
    }
}


void mainDialog::openTaskShow_slot()
{
    ui->mainStackedWidget->setCurrentIndex(1);
    QString style = QString("QPushButton{background-color: rgb(102, 102, 102);border-style:none;border:0px;}");
    ui->saveButton->setStyleSheet(style);
    ui->saveButton->setFlat(true);
    ui->deleButton->setStyleSheet(style);
    ui->deleButton->setFlat(true);
    ui->closeButton->setStyleSheet(style);
    ui->closeButton->setFlat(true);
    basicTableShow();
    personTableShow();
    hostTableShow();
}


void mainDialog::on_treatButton_clicked()
{
    if(g_task_id==0)
    {
        QString strTitle = QString("警告");
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
    disposeDialog dsd(this);
    //自定义tabelmodel
//    disposedialog2 dsd(this);
    dsd.setModal(true);
    dsd.hookMainDialog(this);
    dsd.show();
    dsd.exec();
}

void mainDialog::on_userButton_clicked()
{
    sysManageDialog smd(this);
    smd.setModal(true);
    smd.show();
    smd.exec();
}

void mainDialog::on_openTaskButton_clicked()
{
    taskManageDialog tmd(this);
    tmd.setModal(true);
    tmd.hookMainDialog(this);
    tmd.show();
    tmd.exec();
}

//打开工具集
void mainDialog::on_webscanButton_2_clicked()
{
    toolSetDialog tsd(this);
    tsd.setModal(true);
    tsd.show();
    tsd.exec();
}

//打开webscan
void mainDialog::on_webscanButton_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/web.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QString strTitle = QString("失败");
        QString strMsg = QString("启动程序失败");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }
}

void mainDialog::on_wordButton_clicked()
{
    if(g_task_id==0)
    {
        QString strTitle = QString("警告");
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
    //原来的方法
//    wordDialog wdd(this);
//    wdd.setModal(true);
//    wdd.show();
//    wdd.exec();

    //新方法
//    openWordReport();
    ui->mainStackedWidget->setCurrentIndex(2);
    wordPreview();
}

void mainDialog::basicTableShow()
{
    ui->basicTableWidget->setColumnWidth(0,120);
    ui->basicTableWidget->setColumnWidth(1,315);
    ui->basicTableWidget->setColumnWidth(2,120);
    ui->basicTableWidget->setColumnWidth(3,314);
    ui->basicTableWidget->setSpan(8,1,8,3);
    ui->basicTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->basicTableWidget->horizontalHeader()->setVisible(false);
    ui->basicTableWidget->verticalHeader()->setVisible(false);
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM main_task WHERE id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    if(query.next())
    {
        QString strEventName = query.value(11).toString();
        QDateTime time = query.value(12).toDateTime();
        QString strTime = time.toString("yyyy-MM-dd hh:mm:ss");
        QString strLocation = query.value(13).toString();
        QString strCom = query.value(16).toString();
        QString strKind = query.value(14).toString();
        QString strLevel = query.value(15).toString();
        QString strSysName = query.value(8).toString();
        QString strSysLevel = query.value(9).toString();
        QString strFrom = query.value(10).toString();
        QString strDetail = query.value(17).toString();
        QString strEventCom = query.value(2).toString();
        QString strComPlace = query.value(3).toString();
        QString strprin = query.value(4).toString();
        QString strPhone = query.value(5).toString();
        QString strConnect = query.value(6).toString();
        QString strEmail = query.value(7).toString();

        QTableWidgetItem *item1 = new QTableWidgetItem(strEventName);
        ui->basicTableWidget->setItem(0,1,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(strTime);
        ui->basicTableWidget->setItem(0,3,item2);
        QTableWidgetItem *item3 = new QTableWidgetItem(strLocation);
        ui->basicTableWidget->setItem(1,1,item3);
        QTableWidgetItem *item4 = new QTableWidgetItem(strCom);
        ui->basicTableWidget->setItem(1,3,item4);
        QTableWidgetItem *item5 = new QTableWidgetItem(strKind);
        ui->basicTableWidget->setItem(2,1,item5);
        QTableWidgetItem *item6 = new QTableWidgetItem(strLevel);
        ui->basicTableWidget->setItem(2,3,item6);
        QTableWidgetItem *item7 = new QTableWidgetItem(strSysName);
        ui->basicTableWidget->setItem(3,1,item7);
        QTableWidgetItem *item8 = new QTableWidgetItem(strSysLevel);
        ui->basicTableWidget->setItem(3,3,item8);
        QTableWidgetItem *item9 = new QTableWidgetItem(strFrom);
        ui->basicTableWidget->setItem(4,1,item9);
        QTableWidgetItem *item10 = new QTableWidgetItem(strDetail);
        ui->basicTableWidget->setItem(4,3,item10);
        QTableWidgetItem *item11 = new QTableWidgetItem(strEventCom);
        ui->basicTableWidget->setItem(5,1,item11);
        QTableWidgetItem *item12 = new QTableWidgetItem(strComPlace);
        ui->basicTableWidget->setItem(5,3,item12);
        QTableWidgetItem *item13 = new QTableWidgetItem(strprin);
        ui->basicTableWidget->setItem(6,1,item13);
        QTableWidgetItem *item14 = new QTableWidgetItem(strPhone);
        ui->basicTableWidget->setItem(6,3,item14);
        QTableWidgetItem *item15 = new QTableWidgetItem(strConnect);
        ui->basicTableWidget->setItem(7,1,item15);
        QTableWidgetItem *item16 = new QTableWidgetItem(strEmail);
        ui->basicTableWidget->setItem(7,3,item16);

    }
}

void mainDialog::personTableShow()
{
    ui->dispersonTableWidget->verticalHeader()->setVisible(false);
    ui->dispersonTableWidget->setColumnWidth(0,150);
    ui->dispersonTableWidget->setColumnWidth(1,150);
    ui->dispersonTableWidget->setColumnWidth(2,150);
    ui->dispersonTableWidget->setColumnWidth(3,150);
    ui->dispersonTableWidget->setColumnWidth(4,269);
    ui->dispersonTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dispersonTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QSqlQuery query;
    QString strSql = QString("SELECT * FROM dispose_person WHERE task_id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    int i = 0;
    int rowCount = query.size();
    ui->dispersonTableWidget->setRowCount(rowCount);
    while(query.next())
    {
        QString strName = query.value(1).toString();
        QString strNum = query.value(2).toString();
        QString strPhone = query.value(4).toString();
        QString strPosition = query.value(5).toString();
        QString strCom = query.value(3).toString();

        QTableWidgetItem *item1 = new QTableWidgetItem(strName);
        ui->dispersonTableWidget->setItem(i,0,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(strNum);
        ui->dispersonTableWidget->setItem(i,1,item2);
        QTableWidgetItem *item3 = new QTableWidgetItem(strPhone);
        ui->dispersonTableWidget->setItem(i,2,item3);
        QTableWidgetItem *item4 = new QTableWidgetItem(strPosition);
        ui->dispersonTableWidget->setItem(i,3,item4);
        QTableWidgetItem *item5 = new QTableWidgetItem(strCom);
        ui->dispersonTableWidget->setItem(i,4,item5);
        i++;
    }
}

void mainDialog::hostTableShow()
{
//    ui->hostTableWidget->verticalHeader()->setVisible(false);
//    ui->hostTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->hostTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->hostTableWidget->setColumnWidth(0,180);
//    ui->hostTableWidget->setColumnWidth(1,150);
//    ui->hostTableWidget->setColumnWidth(2,130);
//    ui->hostTableWidget->setColumnWidth(3,130);
//    ui->hostTableWidget->setColumnWidth(4,140);
//    ui->hostTableWidget->setColumnWidth(5,139);

//    QSqlQuery query;
//    QString strSql = QString("SELECT * FROM host_info WHERE id=%1").arg(g_task_id);
//    bool ok = query.exec(strSql);
//    if(!ok)
//    {
//        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
//    }
//    int rowCount = query.size();
//    int i = 0;
//    ui->hostTableWidget->setRowCount(rowCount);
//    if(query.next())
//    {
//        QString strName = query.value(1).toString();
//        QString strIp = query.value(2).toString();
//        QString strkind = query.value(3).toString();
//        QString strSys = query.value(4).toString();

//        QTableWidgetItem *item1 = new QTableWidgetItem(strName);
//        ui->hostTableWidget->setItem(i,0,item1);
//        QTableWidgetItem *item2 = new QTableWidgetItem(strIp);
//        ui->hostTableWidget->setItem(i,1,item2);
//        QTableWidgetItem *item3 = new QTableWidgetItem(strkind);
//        ui->hostTableWidget->setItem(i,2,item3);
//        QTableWidgetItem *item4 = new QTableWidgetItem(strSys);
//        ui->hostTableWidget->setItem(i,3,item4);
//        i++;
//    }

//    strSql = QString("SELECT * FROM check_task WHERE id=%1").arg(g_task_id);
//    ok = query.exec(strSql);
//    if(!ok)
//    {
//        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
//    }
//    if(query.next())
//    {
//        QString strMiddle = query.value(5).toString();
//        QString strDatabase = query.value(4).toString();

//        QTableWidgetItem *item5 = new QTableWidgetItem(strMiddle);
//        ui->hostTableWidget->setItem(0,4,item5);
//        QTableWidgetItem *item6 = new QTableWidgetItem(strDatabase);
//        ui->hostTableWidget->setItem(0,5,item6);
//    }
    ui->hostTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hostTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表头
    ui->hostTableWidget->horizontalHeader()->setSectionsClickable(false);
    ui->hostTableWidget->verticalHeader()->setVisible(false);

    QSqlQuery query1;
    QString strSql = QString("SELECT * FROM check_task WHERE id = %1").arg(g_task_id);
    bool ok = query1.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error:"<<query1.lastError();
    }

    int attcount = query1.size();
    ui->hostTableWidget->setRowCount(attcount);
    int row=0;
    while(query1.next())
    {
        QString name = query1.value(1).toString();
        QString ip = query1.value(2).toString();
        QString kind = query1.value(3).toString();
        QString sys = query1.value(6).toString();
        QString tool = query1.value(7).toString();
        QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1").arg(name));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString("%1").arg(ip));
        QTableWidgetItem *item3 = new QTableWidgetItem(QString("%1").arg(kind));
        QTableWidgetItem *item4 = new QTableWidgetItem(QString("%1").arg(sys));
        QTableWidgetItem *item5 = new QTableWidgetItem(QString("%1").arg(tool));

        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->hostTableWidget->setItem(row,0,item1);
        ui->hostTableWidget->setItem(row,1,item2);
        ui->hostTableWidget->setItem(row,2,item5);
        ui->hostTableWidget->setItem(row,3,item3);
        ui->hostTableWidget->setItem(row,4,item4);
        ++row;
    }
    for(int i=0;i<attcount;i++)
    {
        ui->hostTableWidget->setRowHeight(i,30);
    }

    ui->hostTableWidget->setColumnWidth(1,180);
    ui->hostTableWidget->setColumnWidth(2,180);
    ui->hostTableWidget->setColumnWidth(3,150);
    ui->hostTableWidget->setColumnWidth(0,189);
    ui->hostTableWidget->setColumnWidth(4,170);
}

void mainDialog::refresh_table_slot()
{
    basicTableShow();
    personTableShow();
    hostTableShow();
}

void mainDialog::on_basicModifyButton_clicked()
{
    modifyDialog mdd(this);
    connect(mdd.certainButton,SIGNAL(clicked()),this,SLOT(refresh_table_slot()));
    mdd.setModal(true);
    mdd.show();
    mdd.exec();
}

void mainDialog::on_personAddButton_clicked()
{
//    ifDispose = true;
//    newUserDialog nud(this);
//    connect(nud.certainButton,SIGNAL(clicked()),this,SLOT(refresh_table_slot()));
//    nud.show();
//    nud.exec();
    addDisPersonDialog adpd(this);
    adpd.setModal(true);
    connect(adpd.certainButton,SIGNAL(clicked()),this,SLOT(refresh_table_slot()));
    adpd.show();
    adpd.exec();
}

//修改处置人员
void mainDialog::on_personModifyButton_clicked()
{
    modifyPersonDialog mpw(this);
    connect(mpw.certainButton,SIGNAL(clicked()),this,SLOT(refresh_table_slot()));
    int row = ui->dispersonTableWidget->currentRow();
    if(row==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的人员");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QString num = ui->dispersonTableWidget->item(row,1)->text();
    mpw.getPoliceNum(num);
    mpw.LineEditInit();
    mpw.show();
    mpw.exec();
}

//删除处置人员
void mainDialog::on_personDelButton_clicked()
{
    int row = ui->dispersonTableWidget->currentRow();
    if(row==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的人员");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QString num = ui->dispersonTableWidget->item(row,1)->text();
    QSqlQuery query;
    QString strSql = QString("DELETE FROM dispose_person WHERE task_id=%1 AND police_num='%2'").arg(g_task_id).arg(num);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
        return;
    }
    else
    {
        QString strTitle = QString("成功");
        QString strMsg = QString("已成功删除");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        personTableShow();
    }
}

//添加资产
void mainDialog::on_hostAddButton_clicked()
{
    addHostDialog ahd(this);
    connect(ahd.certainButton,SIGNAL(clicked()),this,SLOT(refresh_table_slot()));
    ahd.show();
    ahd.exec();
}


void mainDialog::on_hostDelButton_clicked()
{
    int row = ui->hostTableWidget->currentRow();
    if(row==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QString name = ui->hostTableWidget->item(row,0)->text();

    QString strTitle = QString("删除");
    QString strMsg = QString("你确定删除%1吗？").arg(name);
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
        return;
    }
    else
    {
        qDebug()<<"delete host id:"<<g_task_id<<"name:"<<name.toStdString().c_str();
        QString action = QString("删除资产");
        QString state = QString("成功");
        operateLog(action,state);
        QString strSql = QString("DELETE FROM host_info WHERE id=%1 AND host_name = '%2'")
                .arg(g_task_id).arg(name);
        QSqlQuery query;
        bool ok=query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<" sql:"<<strSql.toStdString().c_str();
        }
        strSql = QString("DELETE FROM check_task WHERE id=%1 AND host_name='%2'")
                .arg(g_task_id).arg(name);
        ok=query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<" sql:"<<strSql.toStdString().c_str();
        }
        hostTableShow();
    }
}

//任务另存为
void mainDialog::on_saveButton_clicked()
{
    int mTaskId = 0;
    mTaskId = g_task_id;
    QString saveFileName = QFileDialog::getSaveFileName(this,QString("另存为任务"),".","(*.pro)");
    if(saveFileName.isEmpty())
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择文件");
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
        QString state,eventCompany,comAddress,principal,strPhone,officePhone,strEmail,sysName,sysLevel,eventSource,
                eventName,eventTime,eventPlace,eventKind,eventLevel,disCompany,eventDiscribe,savePlace;
        QSqlQuery query;
        QString strSql = QString("SELECT * FROM main_task WHERE id=%1").arg(mTaskId);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql;
        }
        if(query.next())
        {
            state = query.value(1).toString();
            eventCompany = query.value(2).toString();
            comAddress = query.value(3).toString();
            principal = query.value(4).toString();
            strPhone = query.value(5).toString();
            officePhone = query.value(6).toString();
            strEmail = query.value(7).toString();
            sysName = query.value(8).toString();
            sysLevel = query.value(9).toString();
            eventSource = query.value(10).toString();

            eventName = query.value(11).toString();
            eventTime = query.value(12).toString();
            eventPlace = query.value(13).toString();
            eventKind = query.value(14).toString();
            eventLevel = query.value(15).toString();
            disCompany = query.value(16).toString();
            eventDiscribe = query.value(17).toString();
            savePlace = query.value(18).toString();
        }

        QDomDocument doc;
        QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(instruction);
        QDomElement task = doc.createElement("Task");
        doc.appendChild(task);
        QDomElement taskInfo = doc.createElement("task_info");
        taskInfo.setAttribute("taskId",mTaskId);
        taskInfo.setAttribute("state",state);
        task.appendChild(taskInfo);
        QDomElement comInfo = doc.createElement("com_info");
        task.appendChild(comInfo);

        QDomElement comName = doc.createElement("com_name");
        QDomText text_1 = doc.createTextNode(eventCompany);
        comName.appendChild(text_1);
        comInfo.appendChild(comName);

        QDomElement comAddr = doc.createElement("com_addr");
        QDomText text_2 = doc.createTextNode(comAddress);
        comAddr.appendChild(text_2);
        comInfo.appendChild(comAddr);

        QDomElement princi = doc.createElement("principal");
        QDomText text_3 = doc.createTextNode(principal);
        princi.appendChild(text_3);
        comInfo.appendChild(princi);

        QDomElement phone = doc.createElement("phone");
        QDomText text_4 = doc.createTextNode(strPhone);
        phone.appendChild(text_4);
        comInfo.appendChild(phone);

        QDomElement office = doc.createElement("officePhone");
        QDomText text_5 = doc.createTextNode(officePhone);
        office.appendChild(text_5);
        comInfo.appendChild(office);

        QDomElement email = doc.createElement("email");
        QDomText text_6 = doc.createTextNode(strEmail);
        email.appendChild(text_6);
        comInfo.appendChild(email);

        QDomElement systemName = doc.createElement("systemName");
        QDomText text_7 = doc.createTextNode(sysName);
        systemName.appendChild(text_7);
        comInfo.appendChild(systemName);

        QDomElement systemLevel = doc.createElement("systemLevel");
        QDomText text_8 = doc.createTextNode(sysLevel);
        systemLevel.appendChild(text_8);
        comInfo.appendChild(systemLevel);

        QDomElement eventS = doc.createElement("eventSource");
        QDomText text_9 = doc.createTextNode(eventSource);
        eventS.appendChild(text_9);
        comInfo.appendChild(eventS);

        QDomElement dispose_info = doc.createElement("dispose_info");
        task.appendChild(dispose_info);

        QDomElement eventN = doc.createElement("eventName");
        QDomText text_10 = doc.createTextNode(eventName);
        eventN.appendChild(text_10);
        dispose_info.appendChild(eventN);

        QDomElement eventT = doc.createElement("eventTime");
        QDomText text_11 = doc.createTextNode(eventTime);
        eventT.appendChild(text_11);
        dispose_info.appendChild(eventT);

        QDomElement eventP = doc.createElement("eventPlace");
        QDomText text_12 = doc.createTextNode(eventPlace);
        eventP.appendChild(text_12);
        dispose_info.appendChild(eventP);

        QDomElement eventK = doc.createElement("eventKind");
        QDomText text_13 = doc.createTextNode(eventKind);
        eventK.appendChild(text_13);
        dispose_info.appendChild(eventK);

        QDomElement eventL = doc.createElement("eventLevel");
        QDomText text_14 = doc.createTextNode(eventLevel);
        eventL.appendChild(text_14);
        dispose_info.appendChild(eventL);

        QDomElement disCom = doc.createElement("disCompany");
        QDomText text_15 = doc.createTextNode(disCompany);
        disCom.appendChild(text_15);
        dispose_info.appendChild(disCom);

        QDomElement eventDis = doc.createElement("eventDiscribe");
        QDomText text_16 = doc.createTextNode(eventDiscribe);
        eventDis.appendChild(text_16);
        dispose_info.appendChild(eventDis);

        QDomElement svPlace = doc.createElement("savePlace");
        QDomText text_17 = doc.createTextNode(savePlace);
        svPlace.appendChild(text_17);
        dispose_info.appendChild(svPlace);

        QFile file(saveFileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
        {
            qWarning()<<"open file fail ";
            return;
        }
        QTextStream out(&file);
        out.setCodec("UTF-8");
        doc.save(out,4,QDomNode::EncodingFromTextStream);
        file.close();
        QString strTitle = QString("成功");
        QString strMsg = QString("任务已成功保存");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }
}

//任务删除
void mainDialog::on_deleButton_clicked()
{
    int mTaskId = 0;
    mTaskId = g_task_id;
    QSqlQuery query;
    QString strSql;

    QString strTitle = QString("删除");
    QString strMsg = QString("你确定删除当前的任务吗？");
    QString showMsg = "<font color='black'>"+strMsg+"</font>";
    QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok|QMessageBox::No,0);
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
        strSql = QString("DELETE FROM host_info WHERE id=%1").arg(mTaskId);
        query.exec(strSql);
        strSql = QString("DELETE FROM check_task WHERE id=%1").arg(mTaskId);
        query.exec(strSql);
        strSql = QString("DELETE FROM main_task WHERE id=%1").arg(mTaskId);
        query.exec(strSql);
        strSql = QString("DELETE FROM safe_record_form WHERE id=%1").arg(mTaskId);
        query.exec(strSql);
        strSql = QString("DELETE FROM web_record_form WHERE id=%1").arg(mTaskId);
        query.exec(strSql);
        g_task_id = 0;
        ui->mainStackedWidget->setCurrentIndex(0);
    }
}

void mainDialog::log_table_show_slot(QString logIp)
{
    Q_UNUSED(logIp);
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM logreport WHERE task_id=%1")
            .arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err:"<<query.lastError();
        return;
    }
    else
    {
        int row = 0;
        while(query.next())
        {
            ui->logTableWidget->insertRow(row);
            ui->logTableWidget->setRowHeight(row,60);
            QString mCode = query.value(1).toString();
            QTableWidgetItem *item1 = new QTableWidgetItem(mCode);
            ui->logTableWidget->setItem(row,0,item1);
            QString mTime = query.value(2).toString();
            QTableWidgetItem *item2 = new QTableWidgetItem(mTime);
            ui->logTableWidget->setItem(row,1,item2);
            QString mIp = query.value(3).toString();
            QTableWidgetItem *item3 = new QTableWidgetItem(mIp);
            ui->logTableWidget->setItem(row,2,item3);
            QString mDetail = query.value(4).toString();
            QTableWidgetItem *item4 = new QTableWidgetItem(mDetail);
            ui->logTableWidget->setItem(row,3,item4);
            row++;
        }
        connect(ui->logTableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),
                ui->logTableWidget,SLOT(resizeRowsToContents()) );
    }
}

//关闭任务
void mainDialog::on_closeButton_clicked()
{
    //关闭选中的任务
    QString strTitle = QString("关闭任务");
    QString strMsg = QString("你确定关闭当前任务吗？");
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
        return;
    }
    else
    {
        QSqlQuery query;
        int mTaskId = g_task_id;
        QString strSql = QString("SELECT state FROM main_task WHERE id = %1").arg(mTaskId);
        query.exec(strSql);
        if(query.next())
        {
            QString state = query.value(0).toString();
            if(!(state == QString("关闭")))
            {
                strSql = QString("UPDATE main_task SET state = '关闭' WHERE id = %1").arg(mTaskId);
                query.exec(strSql);
                g_task_id = 0;
                ui->mainStackedWidget->setCurrentIndex(0);
            }
        }
    }
}

void mainDialog::on_pushButton_clicked()
{
    ui->mainStackedWidget->setCurrentIndex(0);
}

bool ifHaveIp1(QString ip,QStringList ip_list)
{
    for(int i=0;i<ip_list.size();i++)
    {
        if(ip==ip_list[i])
        {
            return true;
        }
    }
    return false;
}

void analyzeXml1(QString code,QStringList &list)
{
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    //网口工具
    if(code==QString("012")||code==QString("013")||code==QString("011"))
    {
        QString mOnlinePath = settings.value("autoscan/OnlineScanDir").toString();
        QString strId = QString("%1").arg(g_task_id);
        mOnlinePath=mOnlinePath+"\\"+strId+"\\";

        //判断路径是否存在
        QDir dir(mOnlinePath);
        if(!dir.exists())
        {
            qWarning()<<"return xml dir not exist!";
            return;
        }

         //获取所选文件类型过滤器
        QStringList filters;
        filters<<QString("*.xml");

         //定义迭代器并设置过滤器
        QDirIterator dir_iterator(mOnlinePath,
            filters,
            QDir::Files | QDir::NoSymLinks,
            QDirIterator::Subdirectories);
        QStringList online_list;
        while(dir_iterator.hasNext())
        {
            dir_iterator.next();
            QFileInfo file_info =dir_iterator.fileInfo();
            QString absolute_file_path =file_info.absoluteFilePath();
            online_list.append(absolute_file_path);
        }
        QString xmlFile;
        QDomDocument doc;
        for(int i=0;i<online_list.size();i++)
        {
            xmlFile = online_list[i];
            QFile file(xmlFile);
            if (!file.open(QFile::ReadOnly | QFile::Text))
            {
                qWarning()<<"open file error:"<<xmlFile;
                continue;
            }
            doc.setContent(&file);
            file.close();
            //webscan
            if(code==QString("012"))
            {
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch_1=docElem.elementsByTagName("tool_info");
                QDomNode node_1 = nodeswitch_1.item(0);
                QDomElement elemnodeswitch_1=node_1.toElement();
                QString toolCode = qPrintable(elemnodeswitch_1.attribute("toolcategory"));
                if(toolCode!=QString("012"))
                {
                    continue;
                }

                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp = qPrintable(element_2.attribute("dest_ip"));
                list.append(strIp);

                QDomNodeList nodeswitch_3 = docElem.elementsByTagName("operation_info");
                QDomNode node_3 = nodeswitch_3.item(0);
                QDomElement element_3 = node_3.toElement();
                QString strOs = element_3.attribute("src_os");
                list.append(strOs);

                QDomNodeList nodelist=docElem.elementsByTagName("item_name");
                QDomNodeList nodelist_2=docElem.elementsByTagName("high");
                QDomNodeList nodelist_3=docElem.elementsByTagName("middle");
                QDomNodeList nodelist_4=docElem.elementsByTagName("low");
                QDomNode item_node;
                QDomElement item_element;
                for(int i=0;i<nodelist.count();i++)
                {
                    item_node = nodelist.item(i);
                    item_element = item_node.toElement();
                    if(item_element.text()==QString("合计"))
                    {
                        item_node = nodelist_2.item(i);
                        item_element = item_node.toElement();
                        QString strHigh = item_element.text();

                        item_node = nodelist_3.item(i);
                        item_element = item_node.toElement();
                        QString strMiddle = item_element.text();

                        item_node = nodelist_4.item(i);
                        item_element = item_node.toElement();
                        QString strLow = item_element.text();

                        list.append(strHigh);
                        list.append(strMiddle);
                        list.append(strLow);
                        break;
                    }
                }
            }
            else if(code==QString("011"))  //系统漏洞
            {
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch_1=docElem.elementsByTagName("tool_info");
                QDomNode node_1 = nodeswitch_1.item(0);
                QDomElement elemnodeswitch_1=node_1.toElement();
                QString toolCode = qPrintable(elemnodeswitch_1.attribute("toolcategory"));
                if(toolCode!=QString("011"))
                {
                    continue;
                }
                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("ip");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp = element_2.text();
                list.append(strIp);

                QDomNodeList nodeswitch_3 = docElem.elementsByTagName("vscan_type");
                QDomNode node_3 = nodeswitch_3.item(0);
                QDomElement element_3 = node_3.toElement();
                QString strOs = element_3.text();
                list.append(strOs);

                QDomNodeList nodelist=docElem.elementsByTagName("item_Code");
                QDomNodeList nodelist_2=docElem.elementsByTagName("high");
                QDomNodeList nodelist_3=docElem.elementsByTagName("middle");
                QDomNodeList nodelist_4=docElem.elementsByTagName("low");
                QDomNode item_node;
                QDomElement item_element;
                for(int i=0;i<nodelist.count();i++)
                {
                    item_node = nodelist.item(i);
                    item_element = item_node.toElement();
                    if(item_element.text()==QString("Total"))
                    {
                        item_node = nodelist_2.item(i);
                        item_element = item_node.toElement();
                        QString strHigh = item_element.text();

                        item_node = nodelist_3.item(i);
                        item_element = item_node.toElement();
                        QString strMiddle = item_element.text();

                        item_node = nodelist_4.item(i);
                        item_element = item_node.toElement();
                        QString strLow = item_element.text();

                        list.append(strHigh);
                        list.append(strMiddle);
                        list.append(strLow);
                        break;
                    }
                }
            }
            else if(code==QString("013"))     //数据库
            {
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch_1=docElem.elementsByTagName("tool_info");
                QDomNode node_1 = nodeswitch_1.item(0);
                QDomElement elemnodeswitch_1=node_1.toElement();
                QString toolCode = qPrintable(elemnodeswitch_1.attribute("toolcategory"));
                if(toolCode!=QString("013"))
                {
                    continue;
                }

                QDomNodeList nodeswitch_3 = docElem.elementsByTagName("ip");
                QDomNode node_3 = nodeswitch_3.item(0);
                QDomElement element_3 = node_3.toElement();
                QString strIp = element_3.text();
                list.append(strIp);

                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("dbtype");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString dbType = element_2.text();
                list.append(dbType);

                QDomNodeList nodelist=docElem.elementsByTagName("item_threat");
                QDomNode item_node;
                QDomElement item_element;
                int mHigh = 0;
                int mMiddle = 0;
                int mLow = 0;
                for(int i=0;i<nodelist.count();i++)
                {
                    item_node = nodelist.item(i);
                    item_element = item_node.toElement();
                    if(item_element.text()==QString("高"))
                    {
                        ++mHigh;
                    }
                    else if(item_element.text()==QString("中"))
                    {
                        ++mMiddle;
                    }
                    else if(item_element.text()==QString("低"))
                    {
                        ++mLow;
                    }
                }
                QString strHigh = QString::number(mHigh,10);
                QString strMiddle = QString::number(mMiddle,10);
                QString strLow = QString::number(mLow,10);
                list.append(strHigh);
                list.append(strMiddle);
                list.append(strLow);
            }
        }
    }
    else
    {
        QString mOfflinePath = settings.value("autoscan/ScanDir").toString();
        QDir dir(mOfflinePath);
        if(!dir.exists())
        {
            qWarning()<<"dir not exist:"<<mOfflinePath.toStdString().c_str();
            return;
        }

        QStringList filters;
        filters<<QString("*.xml");
        dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
        dir.setNameFilters(filters);

        int dir_count = dir.count();
        if(dir_count <= 0)
        {
            return;
        }

        QStringList file_list;
        //获取分隔符
        QChar separator = QChar('/');
        if(!mOfflinePath.contains(separator))
        {
            separator = QChar('\\');
        }
        QChar last_char = mOfflinePath.at(mOfflinePath.length()-1);
        if(last_char == separator)
        {
            separator = QChar();
        }

        for(int i=0;i<dir_count;i++)
        {
            QString file_name = dir[i];  //文件名称
            QString file_path = mOfflinePath + separator + file_name;   //文件全路径
            file_list.append(file_path);
        }

        QString xmlFile;
        QDomDocument doc;
        for(int i=0;i<file_list.size();i++)
        {
            xmlFile = file_list[i];
            QFile file(xmlFile);
            if (!file.open(QFile::ReadOnly | QFile::Text))
            {
                qWarning()<<"open file error:"<<xmlFile.toStdString().c_str();
                continue;
            }
            doc.setContent(&file);
            file.close();

            QSqlQuery query;
            QString strSql;
            if(code==QString("006"))    //恶意代码
            {
                strSql = QString("SELECT ip_address FROM check_task WHERE id=%1 AND run_tool='网站恶意代码检查工具'")
                        .arg(g_task_id);
                bool ok = query.exec(strSql);
                if(!ok)
                {
                    qWarning()<<"sql error: "<<query.lastError();
                }
                QStringList ip_list;
                QString strIp;
                while(query.next())
                {
                    strIp = query.value(0).toString();
                    ip_list.append(strIp);
                }
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch_1=docElem.elementsByTagName("tool_info");
                QDomNode node_1 = nodeswitch_1.item(0);
                QDomElement elemnodeswitch_1=node_1.toElement();
                QString toolCode = elemnodeswitch_1.attribute("toolcategory");
                if(toolCode!=QString("006"))
                {
                    continue;
                }

                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp_2 = element_2.attribute("src_ip");
                QStringList str_list = strIp_2.split(";");
                QString hostIp = str_list[0];
                if(!ifHaveIp1(hostIp,ip_list))
                {
                    continue;
                }
                list.append(hostIp);

                QDomNodeList nodeswitch_3 = docElem.elementsByTagName("operation_info");
                QDomNode node_3 = nodeswitch_3.item(0);
                QDomElement element_3 = node_3.toElement();
                QString strOs = element_3.attribute("src_os");
                list.append(strOs);

                QDomNodeList nodeswitch_4 = docElem.elementsByTagName("summary");
                QDomNode node_4 = nodeswitch_4.item(0);
                QDomElement element_4 = node_4.toElement();
                QString strNum = element_4.attribute("InfectedFiles");
                list.append(strNum);

                QString strDeatil = QString("异常内容");
                list.append(strDeatil);
            }
            else if(code==QString("004"))   //木马检测
            {
                strSql = QString("SELECT ip_address FROM check_task WHERE id=%1 AND run_tool='木马检查工具'")
                        .arg(g_task_id);
                bool ok = query.exec(strSql);
                if(!ok)
                {
                    qWarning()<<"sql error: "<<query.lastError();
                }
                QStringList ip_list;
                QString strIp;
                while(query.next())
                {
                    strIp = query.value(0).toString();
                    ip_list.append(strIp);
                }
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch_1=docElem.elementsByTagName("tool_info");
                QDomNode node_1 = nodeswitch_1.item(0);
                QDomElement elemnodeswitch_1=node_1.toElement();
                QString toolCode = elemnodeswitch_1.attribute("toolcategory");
                if(toolCode!=QString("004"))
                {
                    continue;
                }
                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp_2 = element_2.attribute("src_ip");
                QStringList str_list = strIp_2.split(";");
                QString hostIp = str_list[0];
                if(!ifHaveIp1(hostIp,ip_list))
                {
                    continue;
                }
                list.append(hostIp);

                QDomNodeList nodeswitch_3 = docElem.elementsByTagName("operation_info");
                QDomNode node_3 = nodeswitch_3.item(0);
                QDomElement element_3 = node_3.toElement();
                QString strOs = element_3.attribute("src_os");
                list.append(strOs);

                QDomNodeList nodeswitch_4 = docElem.elementsByTagName("summary");
                QDomNode node_4 = nodeswitch_4.item(0);
                QDomElement element_4 = node_4.toElement();
                QString strNum = element_4.attribute("InfectedFiles");
                list.append(strNum);

                QString strDeatil = QString("疑似木马");
                list.append(strDeatil);
            }
            else if(code==QString("003"))   //病毒
            {
                strSql = QString("SELECT ip_address FROM check_task WHERE id=%1 AND run_tool='病毒检查工具'")
                        .arg(g_task_id);
                bool ok = query.exec(strSql);
                if(!ok)
                {
                    qWarning()<<"sql error: "<<query.lastError();
                }
                QStringList ip_list;
                QString strIp;
                while(query.next())
                {
                    strIp = query.value(0).toString();
                    ip_list.append(strIp);
                }
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch_1=docElem.elementsByTagName("tool_info");
                QDomNode node_1 = nodeswitch_1.item(0);
                QDomElement elemnodeswitch_1=node_1.toElement();
                QString toolCode = elemnodeswitch_1.attribute("toolcategory");
                if(toolCode!=QString("003"))
                {
                    continue;
                }
                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp_2 = element_2.attribute("src_ip");
                QStringList str_list = strIp_2.split(";");
                QString hostIp = str_list[0];
                if(!ifHaveIp1(hostIp,ip_list))
                {
                    continue;
                }
                list.append(hostIp);

                QDomNodeList nodeswitch_3 = docElem.elementsByTagName("operation_info");
                QDomNode node_3 = nodeswitch_3.item(0);
                QDomElement element_3 = node_3.toElement();
                QString strOs = element_3.attribute("src_os");
                list.append(strOs);

                QDomNodeList nodeswitch_4 = docElem.elementsByTagName("summary");
                QDomNode node_4 = nodeswitch_4.item(0);
                QDomElement element_4 = node_4.toElement();
                QString strNum = element_4.attribute("InfectedFiles");
                list.append(strNum);

                QString strDeatil = QString("疑似病毒");
                list.append(strDeatil);
            }
        }
    }
}


void mainDialog::wordPreview()
{
    ui->hostWTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hostWTableWidget->setColumnWidth(0,151);
    ui->hostWTableWidget->setColumnWidth(1,152);
    ui->hostWTableWidget->setColumnWidth(2,152);
    ui->hostWTableWidget->setColumnWidth(3,152);
    ui->hostWTableWidget->setColumnWidth(4,152);
    ui->hostWTableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setColumnWidth(0,151);
    ui->tableWidget->setColumnWidth(1,152);
    ui->tableWidget->setColumnWidth(2,152);
    ui->tableWidget->setColumnWidth(3,152);
    ui->tableWidget->setColumnWidth(4,152);
    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setColumnWidth(0,189);
    ui->tableWidget_2->setColumnWidth(1,190);
    ui->tableWidget_2->setColumnWidth(2,190);
    ui->tableWidget_2->setColumnWidth(3,190);
    ui->tableWidget_2->verticalHeader()->setVisible(false);

    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setColumnWidth(0,181);
    ui->tableWidget_3->setColumnWidth(1,182);
    ui->tableWidget_3->setColumnWidth(2,132);
    ui->tableWidget_3->setColumnWidth(3,132);
    ui->tableWidget_3->setColumnWidth(4,132);
    ui->tableWidget_3->verticalHeader()->setVisible(false);

    ui->tableWidget_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_4->setColumnWidth(0,181);
    ui->tableWidget_4->setColumnWidth(1,182);
    ui->tableWidget_4->setColumnWidth(2,132);
    ui->tableWidget_4->setColumnWidth(3,132);
    ui->tableWidget_4->setColumnWidth(4,132);
    ui->tableWidget_4->verticalHeader()->setVisible(false);

    ui->tableWidget_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_5->setColumnWidth(0,189);
    ui->tableWidget_5->setColumnWidth(1,190);
    ui->tableWidget_5->setColumnWidth(2,190);
    ui->tableWidget_5->setColumnWidth(3,190);
    ui->tableWidget_5->verticalHeader()->setVisible(false);

    ui->tableWidget_6->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_6->setColumnWidth(0,189);
    ui->tableWidget_6->setColumnWidth(1,190);
    ui->tableWidget_6->setColumnWidth(2,190);
    ui->tableWidget_6->setColumnWidth(3,190);
    ui->tableWidget_6->verticalHeader()->setVisible(false);

    QString lineStyle = QString("QLineEdit{border-width:0;border-style:outset;font:11pt 宋体;}");

    ui->disLineEdit_1->setFocusPolicy(Qt::NoFocus);
    ui->disLineEdit_2->setFocusPolicy(Qt::NoFocus);
    ui->disLineEdit_3->setFocusPolicy(Qt::NoFocus);
    ui->disLineEdit_1->setStyleSheet(lineStyle);
    ui->disLineEdit_2->setStyleSheet(lineStyle);
    ui->disLineEdit_3->setStyleSheet(lineStyle);

    ui->jgLineEdit_1->setFocusPolicy(Qt::NoFocus);
    ui->jgLineEdit_2->setFocusPolicy(Qt::NoFocus);
    ui->jgLineEdit_3->setFocusPolicy(Qt::NoFocus);
    ui->jgLineEdit_4->setFocusPolicy(Qt::NoFocus);
    ui->jgLineEdit_5->setFocusPolicy(Qt::NoFocus);
    ui->jgLineEdit_6->setFocusPolicy(Qt::NoFocus);
    ui->jgLineEdit_1->setStyleSheet(lineStyle);
    ui->jgLineEdit_2->setStyleSheet(lineStyle);
    ui->jgLineEdit_3->setStyleSheet(lineStyle);
    ui->jgLineEdit_4->setStyleSheet(lineStyle);
    ui->jgLineEdit_5->setStyleSheet(lineStyle);
    ui->jgLineEdit_6->setStyleSheet(lineStyle);

    QString textStyle = QString("QTextEdit{border-width:0;border-style:outset;font:11pt 宋体;}");
    ui->proTextEdit->setFocusPolicy(Qt::NoFocus);
    ui->anaTextEdit->setFocusPolicy(Qt::NoFocus);
    ui->resTextEdit->setFocusPolicy(Qt::NoFocus);
    ui->proTextEdit->setStyleSheet(textStyle);
    ui->anaTextEdit->setStyleSheet(textStyle);
    ui->resTextEdit->setStyleSheet(textStyle);

    //日志分析
    ui->logTableWidget->verticalHeader()->setVisible(false);
    for(int i=0;i<3;i++)
    {
        ui->logTableWidget->setColumnWidth(i,120);
    }
    ui->logTableWidget->setColumnWidth(3,399);

    QSqlQuery query2;
    QString strSql2 = QString("SELECT * FROM logreport WHERE task_id=%1")
            .arg(g_task_id);
    bool ok2 = query2.exec(strSql2);
    if(!ok2)
    {
        qWarning()<<"sql err:"<<query2.lastError();
        return;
    }
    else
    {
        int row = 0;
        while(query2.next())
        {
            ui->logTableWidget->insertRow(row);
            ui->logTableWidget->setRowHeight(row,60);
            QString mCode = query2.value(1).toString();
            QTableWidgetItem *item1 = new QTableWidgetItem(mCode);
            ui->logTableWidget->setItem(row,0,item1);
            QString mTime = query2.value(2).toString();
            QTableWidgetItem *item2 = new QTableWidgetItem(mTime);
            ui->logTableWidget->setItem(row,1,item2);
            QString mIp = query2.value(3).toString();
            QTableWidgetItem *item3 = new QTableWidgetItem(mIp);
            ui->logTableWidget->setItem(row,2,item3);
            QString mDetail = query2.value(4).toString();
            QTableWidgetItem *item4 = new QTableWidgetItem(mDetail);
            ui->logTableWidget->setItem(row,3,item4);
            row++;
        }
        connect(ui->logTableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),
                ui->logTableWidget,SLOT(resizeRowsToContents()) );
    }

    //网站描述、网站开发架构基本情况
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM web_record_form WHERE id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    if(query.next())
    {
        QString mChName = query.value(1).toString();
        QString mIp = query.value(2).toString();
        QString mWebsite = query.value(3).toString();
        ui->disLineEdit_1->setText(mChName);
        ui->disLineEdit_1->setText(mIp);
        ui->disLineEdit_1->setText(mWebsite);

        QString mLanguage = query.value(20).toString();
        QString mBuild = query.value(21).toString();
        QString mMiddle = query.value(22).toString();
        QString mData = query.value(23).toString();
        QString mServer = query.value(24).toString();
        QString mRemote = query.value(25).toString();

        ui->jgLineEdit_1->setText(mLanguage);
        ui->jgLineEdit_2->setText(mBuild);
        ui->jgLineEdit_3->setText(mMiddle);
        ui->jgLineEdit_4->setText(mData);
        ui->jgLineEdit_5->setText(mServer);
        ui->jgLineEdit_6->setText(mRemote);
    }

    //网站资产情况
    strSql = QString("SELECT * FROM check_task WHERE id=%1").arg(g_task_id);
    ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    QString mIp,mOperate,mDatabase,mMiddle,mKind;
    if(query.next())
    {
        mIp = query.value(2).toString();
        mOperate = query.value(6).toString();
        mDatabase = query.value(4).toString();
        mMiddle = query.value(5).toString();
        mKind = query.value(3).toString();

        QTableWidgetItem *item1 = new QTableWidgetItem(mIp);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->hostWTableWidget->setItem(0,0,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(mOperate);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->hostWTableWidget->setItem(0,1,item2);
        QTableWidgetItem *item3 = new QTableWidgetItem(mDatabase);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->hostWTableWidget->setItem(0,2,item3);
        QTableWidgetItem *item4 = new QTableWidgetItem(mMiddle);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->hostWTableWidget->setItem(0,3,item4);
        QTableWidgetItem *item5 = new QTableWidgetItem(mKind);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->hostWTableWidget->setItem(0,4,item5);
    }


    //网站安全漏洞
    QStringList scan_list;
    QString toolCode;
    //webscan
    toolCode = QString("012");
    analyzeXml1(toolCode,scan_list);
    if(!scan_list.isEmpty())
    {
        QString str = scan_list[0];
        QTableWidgetItem *item1 = new QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(0,0,item1);

        str = scan_list[1];
        QTableWidgetItem *item2 = new QTableWidgetItem(str);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(0,1,item2);

        str = scan_list[2];
        QTableWidgetItem *item3 = new QTableWidgetItem(str);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(0,2,item3);

        str = scan_list[3];
        QTableWidgetItem *item4 = new QTableWidgetItem(str);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(0,3,item4);

        str = scan_list[4];
        QTableWidgetItem *item5 = new QTableWidgetItem(str);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(0,4,item5);
    }
    scan_list.clear();
    //系统漏扫
    toolCode = QString("011");
    analyzeXml1(toolCode,scan_list);
    if(!scan_list.isEmpty())
    {
        QString str = scan_list[0];
        QTableWidgetItem *item1 = new QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_3->setItem(0,0,item1);

        str = scan_list[1];
        QTableWidgetItem *item2 = new QTableWidgetItem(str);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_3->setItem(0,1,item2);

        str = scan_list[2];
        QTableWidgetItem *item3 = new QTableWidgetItem(str);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_3->setItem(0,2,item3);

        str = scan_list[3];
        QTableWidgetItem *item4 = new QTableWidgetItem(str);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_3->setItem(0,3,item4);

        str = scan_list[4];
        QTableWidgetItem *item5 = new QTableWidgetItem(str);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_3->setItem(0,4,item5);
    }
    scan_list.clear();
    //数据库安全
    toolCode = QString("013");
    analyzeXml1(toolCode,scan_list);
    if(!scan_list.isEmpty())
    {
        QString str = scan_list[0];
        QTableWidgetItem *item1 = new QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_4->setItem(0,0,item1);

        str = scan_list[1];
        QTableWidgetItem *item2 = new QTableWidgetItem(str);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_4->setItem(0,1,item2);

        str = scan_list[2];
        QTableWidgetItem *item3 = new QTableWidgetItem(str);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_4->setItem(0,2,item3);

        str = scan_list[3];
        QTableWidgetItem *item4 = new QTableWidgetItem(str);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_4->setItem(0,3,item4);

        str = scan_list[4];
        QTableWidgetItem *item5 = new QTableWidgetItem(str);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_4->setItem(0,4,item5);
    }
    scan_list.clear();
    //恶意代码
    toolCode = QString("006");
    analyzeXml1(toolCode,scan_list);
    if(!scan_list.isEmpty())
    {
        QString str = scan_list[0];
        QTableWidgetItem *item1 = new QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_2->setItem(0,0,item1);

        str = scan_list[1];
        QTableWidgetItem *item2 = new QTableWidgetItem(str);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_2->setItem(0,1,item2);

        str = scan_list[2];
        QTableWidgetItem *item3 = new QTableWidgetItem(str);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_2->setItem(0,2,item3);

        str = scan_list[3];
        QTableWidgetItem *item4 = new QTableWidgetItem(str);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_2->setItem(0,3,item4);
    }
    scan_list.clear();
    //木马
    toolCode = QString("004");
    analyzeXml1(toolCode,scan_list);
    if(!scan_list.isEmpty())
    {
        QString str = scan_list[0];
        QTableWidgetItem *item1 = new QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_5->setItem(0,0,item1);

        str = scan_list[1];
        QTableWidgetItem *item2 = new QTableWidgetItem(str);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_5->setItem(0,1,item2);

        str = scan_list[2];
        QTableWidgetItem *item3 = new QTableWidgetItem(str);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_5->setItem(0,2,item3);

        str = scan_list[3];
        QTableWidgetItem *item4 = new QTableWidgetItem(str);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_5->setItem(0,3,item4);
    }
    scan_list.clear();
    //病毒
    toolCode = QString("003");
    analyzeXml1(toolCode,scan_list);
    if(!scan_list.isEmpty())
    {
        QString str = scan_list[0];
        QTableWidgetItem *item1 = new QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_6->setItem(0,0,item1);

        str = scan_list[1];
        QTableWidgetItem *item2 = new QTableWidgetItem(str);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_6->setItem(0,1,item2);

        str = scan_list[2];
        QTableWidgetItem *item3 = new QTableWidgetItem(str);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_6->setItem(0,2,item3);

        str = scan_list[3];
        QTableWidgetItem *item4 = new QTableWidgetItem(str);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget_6->setItem(0,3,item4);
    }
    scan_list.clear();


    //应急处置情况
    QString str;
    strSql = QString("SELECT * FROM website_condition WHERE id=%1").arg(g_task_id);
    ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    if(query.next())
    {
        str = query.value(2).toString();
        ui->proTextEdit->setText(str);
        str = query.value(3).toString();
        ui->anaTextEdit->setText(str);
        str = query.value(4).toString();
        ui->resTextEdit->setText(str);
    }
    else
    {
        str = QString("未填写过程");
        ui->proTextEdit->setText(str);

        str = QString("未填写原因");
        ui->anaTextEdit->setText(str);

        str = QString("未填写结果");
        ui->resTextEdit->setText(str);
    }
}

void mainDialog::report_success_slot(int code)
{
    switch (code) {
    case 0:
    {
        QString strTitle = QString("成功");
        QString strMsg = QString("导出报告成功");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))

        {

            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

        }
        WrrMsg->exec();
        break;
    }
    case 1:
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("数据库出错，请检查数据库连接");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        break;
    }
    case 2:
    {
        QString strTitle = QString("加载word模板失败");
        QString strMsg = QString("word模板可能被移除或者被占用");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        break;
    }
    case 3:
    {
        QString strTitle = QString("失败");
        QString strMsg = QString("生成word报告失败，请检查dll文件是否存在");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        break;
    }
    default:
        break;
    }
}

void mainDialog::on_pushButton_3_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,QString("保存文件"),QDir::currentPath(),"(*.docx)");
    if(saveFileName.isEmpty())
    {
        QMessageBox::information(this,QString("出错"),QString("请选择文件"));
        return;
    }
    connect(&reportThread,SIGNAL(ifSuccess(int)),this,SLOT(report_success_slot(int)));
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm ddd");
    reportThread.getInfo(saveFileName,strTime);
    reportThread.test();
}
