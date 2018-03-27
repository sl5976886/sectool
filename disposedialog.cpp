#include "disposedialog.h"
#include "ui_disposedialog.h"
#include "editwebdialog.h"
#include "editsecdialog.h"
#include "editsafedialog.h"
#include "commonfunction.h"
#include "addhostdialog.h"
#include <QButtonGroup>
#include <QScrollBar>
#include <QScrollArea>
#include <QImage>
#include <QFont>
#include <QPushButton>
#include <QBoxLayout>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QRadioButton>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSize>
#include <QSqlError>
#include <QFile>
#include <QFileDialog>
#include <QDomDocument>
#include <QProcess>
#include <QDir>
#include <QLibrary>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QDateTime>
#include <QStringList>
#include <WinSock2.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <Shlwapi.h>
#include <comdef.h>
#include <string>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMovie>
#include <QMutex>
#include <QDirIterator>


disposeDialog::disposeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disposeDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("应急处置"));
    this->setFixedSize(this->width(),this->height());

    ui->stackedWidget->setCurrentIndex(0);
    qInstallMessageHandler(outputMessage);
    QString action = QString("应急处置");
    QString state = QString("成功");
    operateLog(action,state);
    ui->tabWidget->setCurrentIndex(0);
    ui->secondRadioButton->setVisible(false);
    ui->label_7->setVisible(false);
    //表格展现
    safeTableWidgetShow();
    webTableWidgetShow();
    archTableWidgetShow();
    attackTableWidgetShow();
    anaTableWidgetShow();
    eventTableWidgetShow();
    disposeTableWidgetShow();
    antiTableWidgetShow();

    disposeShow();

    QObject::connect(ui->firstRadioButton,SIGNAL(clicked()),this,SLOT(chooseFirstWidgetSlot()));
    QObject::connect(ui->secondRadioButton,SIGNAL(clicked()),this,SLOT(chooseSecondWidgetSlot()));
    ui->firstRadioButton->setVisible(false);


    //setScrollBarSheet();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkToolStart()));
    timer->start(30000);

    timer_2 = new QTimer(this);
    connect(timer_2,SIGNAL(timeout()),this,SLOT(checkResultSlot()));
    timer_2->start(20000);

    //connect(&render,SIGNAL(refresh(QString)),this,SLOT(refresh_tableWidget(QString)));
    render.test();
    ui->logStackedWidget->setCurrentIndex(0);

    logTableShow();
}

disposeDialog::~disposeDialog()
{
    render.stop();
    render.wait();
    if(timer->isActive())
    {
        timer->stop();
    }
    if(timer_2->isActive())
    {
        timer_2->stop();
    }
    delete ui;
}


QMutex mutex1;

void disposeDialog::safeTableWidgetShow()
{
    ui->safeTableWidget->setColumnWidth(0,137);
    ui->safeTableWidget->setColumnWidth(1,223);
    ui->safeTableWidget->setColumnWidth(2,160);
    ui->safeTableWidget->setColumnWidth(3,226);
    for(int i=0;i<14;i++)
    {
        if(i==0||i==8)
        {
            ui->safeTableWidget->setRowHeight(i,28);
        }
        else
        {
            ui->safeTableWidget->setRowHeight(i,40);
        }
    }
    ui->safeTableWidget->setSpan(0,0,1,4);
    ui->safeTableWidget->setSpan(8,0,1,4);
    ui->safeTableWidget->setSpan(7,1,1,3);
    ui->safeTableWidget->setSpan(11,1,1,3);
    QTableWidgetItem *item = new QTableWidgetItem(QString("网站基本情况"));
    item->setBackgroundColor(QColor(0,0,0));
    item->setForeground(QColor(255,255,255));
    item->setFlags((Qt::ItemFlags)0);
    ui->safeTableWidget->setItem(0,0,item);
    QTableWidgetItem *item1 = new QTableWidgetItem(QString("事件基本情况"));
    item1->setBackgroundColor(QColor(0,0,0));
    item1->setForeground(QColor(255,255,255));
    item1->setFlags((Qt::ItemFlags)0);
    ui->safeTableWidget->setItem(8,0,item1);

    QSqlQuery query;
    QString strSql = QString("SELECT * FROM safe_record_form WHERE id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    if(query.next())
    {
        QString strName = query.value(1).toString();
        QString strContact = query.value(2).toString();
        QString strSystem = query.value(3).toString();
        QString strLevel = query.value(4).toString();
        QString strWeb = query.value(5).toString();
        QString strIp = query.value(6).toString();
        QString strService = query.value(7).toString();
        QString strData = query.value(8).toString();
        QString strOperate = query.value(9).toString();
        QString strServer = query.value(10).toString();
        QString strData2 = query.value(11).toString();
        QString strThird = query.value(12).toString();
        QString strTech = query.value(13).toString();
        QDateTime dateTime = query.value(14).toDateTime();
        QString strOri = query.value(15).toString();
        QString strAtt = query.value(16).toString();
        QDateTime dateTime2 = query.value(17).toDateTime();
        QString strAtta = query.value(18).toString();
        QString strEffect = query.value(19).toString();
        QString strTimeAndPlace = query.value(20).toString();
        QString strDisCom = query.value(21).toString();
        QString strDisPerson = query.value(22).toString();

        QTableWidgetItem *item1 = new QTableWidgetItem(strName);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(1,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(strContact);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(1,3,item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(strSystem);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(2,1,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(strLevel);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(2,3,item4);

        QTableWidgetItem *item5 = new QTableWidgetItem(strWeb);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(3,1,item5);

        QTableWidgetItem *item6 = new QTableWidgetItem(strIp);
        item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(3,3,item6);

        QTableWidgetItem *item7 = new QTableWidgetItem(strService);
        item7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(4,1,item7);

        QTableWidgetItem *item8 = new QTableWidgetItem(strData);
        item8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(4,3,item8);

        QTableWidgetItem *item9 = new QTableWidgetItem(strOperate);
        item9->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(5,1,item9);

        QTableWidgetItem *item10 = new QTableWidgetItem(strServer);
        item10->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(5,3,item10);

        QTableWidgetItem *item11 = new QTableWidgetItem(strData2);
        item11->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(6,1,item11);

        QTableWidgetItem *item12 = new QTableWidgetItem(strThird);
        item12->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(6,3,item12);

        QTableWidgetItem *item13 = new QTableWidgetItem(strTech);
        item13->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(7,1,item13);

        QTableWidgetItem *item14 = new QTableWidgetItem(strAtta);
        item14->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(11,1,item14);

        QTableWidgetItem *item15 = new QTableWidgetItem(strEffect);
        item15->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(12,1,item15);

        QTableWidgetItem *item16 = new QTableWidgetItem(strTimeAndPlace);
        item16->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(12,3,item16);

        QTableWidgetItem *item17 = new QTableWidgetItem(strDisCom);
        item17->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(13,1,item17);

        QTableWidgetItem *item18 = new QTableWidgetItem(strDisPerson);
        item18->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(13,3,item18);

        if(dateTime.isValid())
        {
            date_1 = new QDateEdit(this);
            date_1->setDateTime(dateTime);
            date_1->setCalendarPopup(true);
            ui->safeTableWidget->setCellWidget(9,1,date_1);
        }
        else
        {
            date_1 = new QDateEdit(QDate::currentDate(),this);
            date_1->setCalendarPopup(true);
            ui->safeTableWidget->setCellWidget(9,1,date_1);
        }

        QTableWidgetItem *item19 = new QTableWidgetItem(strOri);
        item19->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(9,3,item19);

        QTableWidgetItem *item20 = new QTableWidgetItem(strAtt);
        item20->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(10,1,item20);

        if(dateTime2.isValid())
        {
            date_2 = new QDateEdit(this);
            date_2->setDateTime(dateTime2);
            date_2->setCalendarPopup(true);
            ui->safeTableWidget->setCellWidget(10,3,date_2);
        }
        else
        {
            date_2 = new QDateEdit(QDate::currentDate(),this);
            date_2->setCalendarPopup(true);
            ui->safeTableWidget->setCellWidget(10,3,date_2);
        }
    }
    else
    {
        date_1 = new QDateEdit(QDate::currentDate(),this);
        date_1->setCalendarPopup(true);
        ui->safeTableWidget->setCellWidget(9,1,date_1);
        date_2 = new QDateEdit(QDate::currentDate(),this);
        date_2->setCalendarPopup(true);
        ui->safeTableWidget->setCellWidget(10,3,date_2);

        //注意！如果不进行初始化后面调用Item时会崩溃！！！
        QString strName = QString("");
        QString strContact = QString("");
        QString strSystem = QString("");
        QString strLevel = QString("");
        QString strWeb = QString("");
        QString strIp = QString("");
        QString strService = QString("");
        QString strData = QString("");
        QString strOperate = QString("");
        QString strServer = QString("");
        QString strData2 = QString("");
        QString strThird = QString("");
        QString strTech = QString("");
        QString strOri = QString("");
        QString strAtt = QString("");
        QString strAtta = QString("");
        QString strEffect = QString("");
        QString strTimeAndPlace = QString("");
        QString strDisCom = QString("");
        QString strDisPerson = QString("");

        QTableWidgetItem *item1 = new QTableWidgetItem(strName);
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(1,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(strContact);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(1,3,item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(strSystem);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(2,1,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(strLevel);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(2,3,item4);

        QTableWidgetItem *item5 = new QTableWidgetItem(strWeb);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(3,1,item5);

        QTableWidgetItem *item6 = new QTableWidgetItem(strIp);
        item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(3,3,item6);

        QTableWidgetItem *item7 = new QTableWidgetItem(strService);
        item7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(4,1,item7);

        QTableWidgetItem *item8 = new QTableWidgetItem(strData);
        item8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(4,3,item8);

        QTableWidgetItem *item9 = new QTableWidgetItem(strOperate);
        item9->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(5,1,item9);

        QTableWidgetItem *item10 = new QTableWidgetItem(strServer);
        item10->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(5,3,item10);

        QTableWidgetItem *item11 = new QTableWidgetItem(strData2);
        item11->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(6,1,item11);

        QTableWidgetItem *item12 = new QTableWidgetItem(strThird);
        item12->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(6,3,item12);

        QTableWidgetItem *item13 = new QTableWidgetItem(strTech);
        item13->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(7,1,item13);

        QTableWidgetItem *item14 = new QTableWidgetItem(strAtta);
        item14->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(11,1,item14);

        QTableWidgetItem *item15 = new QTableWidgetItem(strEffect);
        item15->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(12,1,item15);

        QTableWidgetItem *item16 = new QTableWidgetItem(strTimeAndPlace);
        item16->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(12,3,item16);

        QTableWidgetItem *item17 = new QTableWidgetItem(strDisCom);
        item17->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(13,1,item17);

        QTableWidgetItem *item18 = new QTableWidgetItem(strDisPerson);
        item18->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(13,3,item18);

        QTableWidgetItem *item19 = new QTableWidgetItem(strOri);
        item19->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(9,3,item19);

        QTableWidgetItem *item20 = new QTableWidgetItem(strAtt);
        item20->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->safeTableWidget->setItem(10,1,item20);
    }
}

void disposeDialog::hookMainDialog(mainDialog *w)
{
    mmw = w;
}

void disposeDialog::webTableWidgetShow()
{
    ui->webTableWidget->setColumnWidth(0,118);
    ui->webTableWidget->setColumnWidth(1,245);
    ui->webTableWidget->setColumnWidth(2,133);
    ui->webTableWidget->setColumnWidth(3,245);
    for(int i=0;i<11;i++)
    {
        if(i==3||i==4||i==5||i==8||i==9)
        {
            ui->webTableWidget->setRowHeight(i,50);
        }
        else
        {
            ui->webTableWidget->setRowHeight(i,25);
        }
    }
    ui->webTableWidget->setSpan(1,1,1,3);
    ui->webTableWidget->setSpan(5,1,1,3);
    ui->webTableWidget->setSpan(10,1,1,3);

    QSqlQuery query;
    QString strSql = QString("SELECT * FROM web_record_form WHERE id = %1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    QString mStr;
    if(query.next())
    {
        mStr = query.value(1).toString();
        ui->webTableWidget->item(0,1)->setText(mStr);
        mStr = query.value(2).toString();
        ui->webTableWidget->item(0,3)->setText(mStr);
        mStr = query.value(3).toString();
        ui->webTableWidget->item(1,1)->setText(mStr);
        mStr = query.value(4).toString();
        ui->webTableWidget->item(2,1)->setText(mStr);
        mStr = query.value(5).toString();
        ui->webTableWidget->item(2,3)->setText(mStr);
        mStr = query.value(6).toString();
        ui->webTableWidget->item(3,1)->setText(mStr);
        mStr = query.value(7).toString();
        ui->webTableWidget->item(3,3)->setText(mStr);
        mStr = query.value(8).toString();
        ui->webTableWidget->item(4,1)->setText(mStr);
        mStr = query.value(9).toString();
        ui->webTableWidget->item(4,3)->setText(mStr);
        mStr = query.value(10).toString();
        ui->webTableWidget->item(5,1)->setText(mStr);
        mStr = query.value(11).toString();
        ui->webTableWidget->item(6,1)->setText(mStr);
        mStr = query.value(12).toString();
        ui->webTableWidget->item(6,3)->setText(mStr);
        mStr = query.value(13).toString();
        ui->webTableWidget->item(7,1)->setText(mStr);
        mStr = query.value(14).toString();
        ui->webTableWidget->item(7,3)->setText(mStr);
        mStr = query.value(15).toString();
        ui->webTableWidget->item(8,1)->setText(mStr);
        mStr = query.value(16).toString();
        ui->webTableWidget->item(8,3)->setText(mStr);
        mStr = query.value(17).toString();
        ui->webTableWidget->item(9,1)->setText(mStr);
        mStr = query.value(18).toString();
        ui->webTableWidget->item(9,3)->setText(mStr);
        mStr = query.value(19).toString();
        ui->webTableWidget->item(10,1)->setText(mStr);
    }
}

void disposeDialog::archTableWidgetShow()
{
    ui->archTableWidget->setColumnWidth(0,118);
    ui->archTableWidget->setColumnWidth(1,255);
    ui->archTableWidget->setColumnWidth(2,113);
    ui->archTableWidget->setColumnWidth(3,255);
    ui->archTableWidget->setSpan(1,1,1,3);
    ui->archTableWidget->setSpan(2,1,1,3);
    ui->archTableWidget->setSpan(3,1,1,3);
    ui->archTableWidget->setSpan(4,1,1,4);
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM web_record_form WHERE id = %1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    QString mStr;
    if(query.next())
    {
        mStr = query.value(20).toString();
        ui->archTableWidget->item(0,1)->setText(mStr);
        mStr = query.value(21).toString();
        ui->archTableWidget->item(0,3)->setText(mStr);
        mStr = query.value(22).toString();
        ui->archTableWidget->item(1,1)->setText(mStr);
        mStr = query.value(23).toString();
        ui->archTableWidget->item(2,1)->setText(mStr);
        mStr = query.value(24).toString();
        ui->archTableWidget->item(3,1)->setText(mStr);
        mStr = query.value(25).toString();
        ui->archTableWidget->item(4,1)->setText(mStr);
    }

}

void disposeDialog::antiTableWidgetShow()
{
    ui->antiTableWidget->setColumnWidth(0,120);
    ui->antiTableWidget->setColumnWidth(1,621);
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM web_record_form WHERE id = %1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    QString mStr;
    QTableWidgetItem *item;
    if(query.next())
    {
        mStr = query.value(26).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(0,1,item);
        mStr = query.value(27).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(1,1,item);
        mStr = query.value(28).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(2,1,item);
        mStr = query.value(29).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(3,1,item);
        mStr = query.value(30).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(4,1,item);
        mStr = query.value(31).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(5,1,item);
        mStr = query.value(32).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(6,1,item);
        mStr = query.value(33).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(7,1,item);
        mStr = query.value(34).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(8,1,item);
        mStr = query.value(35).toString();
        item = new QTableWidgetItem(mStr);
        ui->antiTableWidget->setItem(9,1,item);
    }
}

void disposeDialog::attackTableWidgetShow()
{
    ui->attackTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->attackTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表头
    ui->attackTableWidget->horizontalHeader()->setSectionsClickable(false);


    QSqlQuery query1;
    QString strSql = QString("SELECT * FROM check_task WHERE id = %1").arg(g_task_id);
    bool ok = query1.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error:"<<query1.lastError();
    }
    attcount=query1.size();
    if(attcount == 0)
    {
        ui->attackTableWidget->setRowCount(1);
        ui->attackTableWidget->setSpan(0,0,1,6);
        QTableWidgetItem *item = new QTableWidgetItem(QString("没有资产，请添加！"));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->attackTableWidget->setItem(0,0,item);
    }
    else
    {
        int rowCount = ui->attackTableWidget->rowCount();
        if(rowCount==1)
        {
            QString str_1 = ui->attackTableWidget->item(0,0)->text();
            if(str_1==QString("没有资产，请添加！"))
            {
                ui->attackTableWidget->removeRow(0);
            }
        }
        ui->attackTableWidget->setRowCount(attcount);
        int row=0;
        while(query1.next())
        {
            QString name = query1.value(1).toString();
            QString ip = query1.value(2).toString();
            QString kind = query1.value(3).toString();
            QString sys = query1.value(6).toString();
            QString tool = query1.value(7).toString();
            int state = query1.value(8).toInt();
            QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1").arg(name));
            QTableWidgetItem *item2 = new QTableWidgetItem(QString("%1").arg(ip));
            QTableWidgetItem *item3 = new QTableWidgetItem(QString("%1").arg(kind));
            QTableWidgetItem *item4 = new QTableWidgetItem(QString("%1").arg(sys));
            QTableWidgetItem *item5 = new QTableWidgetItem(QString("%1").arg(tool));
            QTableWidgetItem *item6;
            if(tool==QString("SQL注入验证工具"))
            {
                item6 = new QTableWidgetItem(QString("请自行验证"));
            }
            else if(tool==QString("Windows主机配置检查工具")||
                    tool==QString("病毒检查工具")||
                    tool==QString("木马检查工具")||
                    tool==QString("网站恶意代码检查工具"))
            {
                if(state==0)
                {
                    item6 = new QTableWidgetItem(QString("未完成"));
                }
                else if(state==2)
                {
                    item6 = new QTableWidgetItem(QString("已完成"));
                }
                else if(state==3)
                {
                    item6 = new QTableWidgetItem(QString("已关联"));
                }
            }
            else
            {
                if(state==0)
                {
                    item6 = new QTableWidgetItem(QString("等待启动"));
                }
                else if(state==1)
                {
                    item6 = new QTableWidgetItem(QString("已下发"));
                }
                else if(state==2)
                {
                    item6 = new QTableWidgetItem(QString("已完成"));
                }
                else if(state==3)
                {
                    item6 = new QTableWidgetItem(QString("已关联"));
                }
            }

            item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->attackTableWidget->setItem(row,0,item1);
            ui->attackTableWidget->setItem(row,1,item2);
            ui->attackTableWidget->setItem(row,2,item5);
            ui->attackTableWidget->setItem(row,3,item3);
            ui->attackTableWidget->setItem(row,4,item4);
            ui->attackTableWidget->setItem(row,5,item6);
            ++row;
        }
        for(int i=0;i<attcount;i++)
        {
            ui->attackTableWidget->setRowHeight(i,40);
        }
    }
    ui->attackTableWidget->setColumnWidth(1,120);
    ui->attackTableWidget->setColumnWidth(2,180);
    ui->attackTableWidget->setColumnWidth(3,150);
    ui->attackTableWidget->setColumnWidth(0,120);
    ui->attackTableWidget->setColumnWidth(4,110);
    ui->attackTableWidget->setColumnWidth(5,82);
}

void disposeDialog::anaTableWidgetShow()
{

    ui->attackTableWidget->setRowHeight(0,40);
//    ui->anaTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section "
//                                                             "{background-color:rgb(240,240,240);"
//                                                             "}");
    ui->anaTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->anaTableWidget->horizontalHeader()->setSectionsClickable(false);
    QFont font = QFont("Helvetica");
    font.setPointSize(8);

    QSqlQuery query;
    QString strSql = QString("SELECT * FROM host_info WHERE id = %1").arg(g_task_id);
    bool ok =  query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error:"<<query.lastError();
    }
    int count = query.size();
    ui->anaTableWidget->setRowCount(count);
    int row=0;
    while(query.next())
    {
        QString name = query.value(1).toString();
        QString ip = query.value(2).toString();
        QString kind = query.value(3).toString();
        QString sys = query.value(4).toString();
        QString state = query.value(5).toString();
        QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1").arg(name));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString("%1").arg(ip));
        QTableWidgetItem *item3 = new QTableWidgetItem(QString("%1").arg(kind));
        QTableWidgetItem *item4 = new QTableWidgetItem(QString("%1").arg(sys));
        QTableWidgetItem *item5 = new QTableWidgetItem(QString("%1").arg(state));
        item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->anaTableWidget->setItem(row,0,item1);
        ui->anaTableWidget->setItem(row,1,item2);
        ui->anaTableWidget->setItem(row,2,item3);
        ui->anaTableWidget->setItem(row,3,item4);
        ui->anaTableWidget->setItem(row,4,item5);
        ++row;
    }
    ui->anaTableWidget->setColumnWidth(0,150);
    ui->anaTableWidget->setColumnWidth(1,200);
    ui->anaTableWidget->setColumnWidth(2,170);
    ui->anaTableWidget->setColumnWidth(3,120);
    ui->anaTableWidget->setColumnWidth(4,126);

}


void disposeDialog::disposeTableWidgetShow()
{
    for(int i=0;i<3;i++)
    {
        ui->disposeTableWidget->setRowHeight(i,74);
    }
    ui->disposeTableWidget->setColumnWidth(0,132);
    ui->disposeTableWidget->setColumnWidth(1,633);
    QBoxLayout *labelLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    labelLayout->setMargin(3);
    labelLayout->addSpacing(5);

    edit = new QTextEdit(this);
    edit->resize(617,68);
    edit->setPlaceholderText(QString("事件发生过程：(最多可输入1000字符)"));
    labelLayout->addWidget(edit);
    labelLayout->addSpacing(5);
    QLabel *w1 = new QLabel(this);
    w1->setLayout(labelLayout);
    ui->disposeTableWidget->setCellWidget(0,1,w1);

    QBoxLayout *labelLayout_2 = new QBoxLayout(QBoxLayout::LeftToRight);
    labelLayout_2->setMargin(3);
    labelLayout_2->addSpacing(5);
    edit_2 = new QTextEdit(this);
    edit_2->resize(617,68);
    edit_2->setPlaceholderText(QString("详细报告事件(网页被篡改、被种植木马、服务器被控制、发现病毒、拒绝服务攻击等)成因：(最多可输入1000字符)"));

    labelLayout_2->addWidget(edit_2);
    labelLayout_2->addSpacing(5);
    QLabel *w2 = new QLabel(this);
    w2->setLayout(labelLayout_2);
    ui->disposeTableWidget->setCellWidget(1,1,w2);

    QBoxLayout *labelLayout_3 = new QBoxLayout(QBoxLayout::LeftToRight);
    labelLayout_3->setMargin(3);
    labelLayout_3->addSpacing(5);
    edit_3 = new QTextEdit(this);
    edit_3->resize(617,68);
    edit_3->setPlaceholderText(QString("详细报告公安机关开展处置情况、责任单位处置结果及网站系统落实整改情况：(最多可输入1000字符)"));

    labelLayout_3->addWidget(edit_3);
    labelLayout_3->addSpacing(5);
    QLabel *w3 = new QLabel(this);
    w3->setLayout(labelLayout_3);
    ui->disposeTableWidget->setCellWidget(2,1,w3);
}

void disposeDialog::eventTableWidgetShow()
{
}


void disposeDialog::chooseFirstWidgetSlot()
{
    if(ui->firstRadioButton->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void disposeDialog::chooseSecondWidgetSlot()
{
    if(ui->secondRadioButton->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}


void disposeDialog::setScrollBarSheet()
{
    ui->myScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                         "{"
                                                         "width:8px;"
                                                         "background:rgba(0,0,0,0%);"
                                                         "margin:0px,0px,0px,0px;"
                                                         "padding-top:9px;"
                                                         "padding-bottom:9px;"
                                                         "}"
                                                         "QScrollBar::handle:vertical"
                                                         "{"
                                                         "width:8px;"
                                                         "background:rgba(0,0,0,25%);"
                                                         " border-radius:4px;"
                                                         "min-height:20;"
                                                         "}"
                                                         "QScrollBar::handle:vertical:hover"
                                                         "{"
                                                         "width:8px;"
                                                         "background:rgba(0,0,0,50%);"
                                                         " border-radius:4px;"
                                                         "min-height:20;"
                                                         "}"
                                                         "QScrollBar::add-line:vertical"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/3.png);"
                                                         "subcontrol-position:bottom;"
                                                         "}"
                                                         "QScrollBar::sub-line:vertical"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/1.png);"
                                                         "subcontrol-position:top;"
                                                         "}"
                                                         "QScrollBar::add-line:vertical:hover"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/4.png);"
                                                         "subcontrol-position:bottom;"
                                                         "}"
                                                         "QScrollBar::sub-line:vertical:hover"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/2.png);"
                                                         "subcontrol-position:top;"
                                                         "}"
                                                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                         "{"
                                                         "background:rgba(0,0,0,10%);"
                                                         "border-radius:4px;"
                                                         "}");
}

//刷新safetable
void disposeDialog::refresh_safeTable()
{
    int length = webList.size();
    if(length>0)
    {
        QString str_1 = webList[0];
        QTableWidgetItem *item_1 = new QTableWidgetItem(str_1);
        item_1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(0,1,item_1);
    }

    if(length>1)
    {
        QString str_2 = webList[1];
        QTableWidgetItem *item_2 = new QTableWidgetItem(str_2);
        item_2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(0,3,item_2);
    }

    if(length>2)
    {
        QString str_3 = webList[2];
        QTableWidgetItem *item_3 = new QTableWidgetItem(str_3);
        item_3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(1,1,item_3);
    }

    if(length>3)
    {
        QString str_4 = webList[3];
        QTableWidgetItem *item_4 = new QTableWidgetItem(str_4);
        item_4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(2,1,item_4);
    }

    if(length>4)
    {
        QString str_5 = webList[4];
        QTableWidgetItem *item_5 = new QTableWidgetItem(str_5);
        item_5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(2,3,item_5);
    }

    if(length>5)
    {
        QString str_6 = webList[5];
        QTableWidgetItem *item_6 = new QTableWidgetItem(str_6);
        item_6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(3,1,item_6);
    }

    if(length>6)
    {
        QString str_7 = webList[6];
        QTableWidgetItem *item_7 = new QTableWidgetItem(str_7);
        item_7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(3,3,item_7);
    }

    if(length>7)
    {
        QString str_8 = webList[7];
        QTableWidgetItem *item_8 = new QTableWidgetItem(str_8);
        item_8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(4,1,item_8);
    }

    if(length>8)
    {
        QString str_9 = webList[8];
        QTableWidgetItem *item_9 = new QTableWidgetItem(str_9);
        item_9->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(4,3,item_9);
    }

    if(length>9)
    {
        QString str_10 = webList[9];
        QTableWidgetItem *item_10 = new QTableWidgetItem(str_10);
        item_10->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(5,1,item_10);
    }

    if(length>10)
    {
        QString str_11 = webList[10];
        QTableWidgetItem *item_11 = new QTableWidgetItem(str_11);
        item_11->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(6,1,item_11);
    }

    if(length>11)
    {
        QString str_12 = webList[11];
        QTableWidgetItem *item_12 = new QTableWidgetItem(str_12);
        item_12->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(6,3,item_12);
    }

    if(length>12)
    {
        QString str_13 = webList[12];
        QTableWidgetItem *item_13 = new QTableWidgetItem(str_13);
        item_13->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(7,1,item_13);
    }

    if(length>13)
    {
        QString str_14 = webList[13];
        QTableWidgetItem *item_14 = new QTableWidgetItem(str_14);
        item_14->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(7,3,item_14);
    }

    if(length>14)
    {
        QString str_15 = webList[14];
        QTableWidgetItem *item_15 = new QTableWidgetItem(str_15);
        item_15->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(8,1,item_15);
    }

    if(length>15)
    {
        QString str_16 = webList[15];
        QTableWidgetItem *item_16 = new QTableWidgetItem(str_16);
        item_16->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(8,3,item_16);
    }

    if(length>16)
    {
        QString str_17 = webList[16];
        QTableWidgetItem *item_17 = new QTableWidgetItem(str_17);
        item_17->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(9,1,item_17);
    }

    if(length>17)
    {
        QString str_18 = webList[17];
        QTableWidgetItem *item_18 = new QTableWidgetItem(str_18);
        item_18->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(9,3,item_18);
    }

    if(length>18)
    {
        QString str_19 = webList[18];
        QTableWidgetItem *item_19 = new QTableWidgetItem(str_19);
        item_19->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->webTableWidget->setItem(10,1,item_19);
    }
}

void disposeDialog::webDataUpdate()
{
    webList.clear();
    if(ui->webTableWidget->item(0,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(0,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(0,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(0,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(1,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(1,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(2,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(2,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(2,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(2,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(3,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(3,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(3,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(3,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(4,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(4,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(4,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(4,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(5,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(5,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(6,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(6,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(6,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(6,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(7,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(7,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(7,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(7,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(8,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(8,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(8,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(8,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(9,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(9,1)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(9,3)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(9,3)->text());
    }
    else
    {
        webList.append("");
    }
    if(ui->webTableWidget->item(10,1)->text().length()>0)
    {
        webList.append(ui->webTableWidget->item(10,1)->text());
    }
    else
    {
        webList.append("");
    }
}

//编辑网站管理基本信息
void disposeDialog::on_webEditButton_clicked()
{
    webDataUpdate();
    editWebDialog ewd(this);
    ewd.setModal(true);
    connect(ewd.certainButton,SIGNAL(clicked()),this,SLOT(refresh_safeTable()));
    ewd.show();
    ewd.exec();
}

//刷新secTable
void disposeDialog::refresh_secTable()
{
    int len = secList.size();
    QString str = QString("");
    QTableWidgetItem *item;
    if(len>0)
    {
        str = secList[0];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->archTableWidget->setItem(0,1,item);
    }
    if(len>1)
    {
        str = secList[1];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->archTableWidget->setItem(0,3,item);
    }
    if(len>2)
    {
        str = secList[2];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->archTableWidget->setItem(1,1,item);
    }
    if(len>3)
    {
        str = secList[3];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->archTableWidget->setItem(2,1,item);
    }
    if(len>4)
    {
        str = secList[4];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->archTableWidget->setItem(3,1,item);
    }
    if(len>5)
    {
        str = secList[5];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->archTableWidget->setItem(4,1,item);
    }
}

//编辑网站开发架构
void disposeDialog::on_secEditButton_clicked()
{
    editSecDialog esd(this);
    connect(esd.certainButton,SIGNAL(clicked()),this,SLOT(refresh_secTable()));
    esd.setModal(true);
    esd.show();
    esd.exec();
}

void disposeDialog::safeTableData()
{
    if(g_task_id!=0)
    {
        QSqlQuery query;
        QString strSql = QString("SELECT * FROM safe_record_form WHERE id = %1").arg(g_task_id);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            //error log
        }
        else if(query.next())
        {
            QString name = query.value(1).toString();
            QTableWidgetItem *item = new QTableWidgetItem(name);
            ui->safeTableWidget->setItem(1,1,item);
            QString person = query.value(2).toString();
            QTableWidgetItem *item1 = new QTableWidgetItem(person);
            ui->safeTableWidget->setItem(1,3,item1);
        }
        else
        {
        }
    }
}

//刷新antiTable
void disposeDialog::refresh_antiTable()
{
    int len = safeList.size();
    QString str;
    QTableWidgetItem *item;
    if(len>0)
    {
        str = safeList[0];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(0,1,item);
    }
    if(len>1)
    {
        str = safeList[1];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(1,1,item);
    }
    if(len>2)
    {
        str = safeList[2];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(2,1,item);
    }
    if(len>3)
    {
        str = safeList[3];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(3,1,item);
    }
    if(len>4)
    {
        str = safeList[4];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(4,1,item);
    }
    if(len>5)
    {
        str = safeList[5];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(5,1,item);
    }
    if(len>6)
    {
        str = safeList[6];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(6,1,item);
    }
    if(len>7)
    {
        str = safeList[7];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(7,1,item);
    }
    if(len>8)
    {
        str = safeList[8];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(8,1,item);
    }
    if(len>9)
    {
        str = safeList[9];
        item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->antiTableWidget->setItem(9,1,item);
    }
}

//安全防护编辑
void disposeDialog::on_antiEditButton_clicked()
{
    editSafeDialog esd(this);
    esd.setModal(true);
    connect(esd.certainButton,SIGNAL(clicked()),this,SLOT(refresh_antiTable()));
    esd.show();
    esd.exec();
}

//刷新attack table
void disposeDialog::refresh_attackTable()
{
    attackTableWidgetShow();
    anaTableWidgetShow();
}

//添加资产
void disposeDialog::on_addHostButton_clicked()
{
    addHostDialog ahd(this);
    connect(ahd.certainButton,SIGNAL(clicked()),this,SLOT(refresh_attackTable()));
    ahd.show();
    ahd.exec();
}

QString disposeDialog::toolCode(QString toolName)
{
    QString strCode;
    if(toolName==QString("Windows主机配置检查工具"))
    {
        strCode = QString("001");
        return strCode;
    }
    else if(toolName==QString("病毒检查工具"))
    {
        strCode = QString("003");
        return strCode;
    }
    else if(toolName==QString("木马检查工具"))
    {
        strCode = QString("004");
        return strCode;
    }
    else if(toolName==QString("网站恶意代码检查工具"))
    {
        strCode = QString("006");
        return strCode;
    }
    else if(toolName==QString("系统漏洞检查工具"))
    {
        strCode = QString("011");
        return strCode;
    }
    else if(toolName==QString("网站安全检查工具"))
    {
        strCode = QString("012");
        return strCode;
    }
    else if(toolName==QString("数据库安全检查工具"))
    {
        strCode = QString("013");
        return strCode;
    }
    else if(toolName==QString("Linux主机配置检查工具"))
    {
        strCode = QString("002");
        return strCode;
    }
    else if(toolName==QString("SQL注入验证工具"))
    {
        strCode = QString("010");
        return strCode;
    }
    else
    {
        strCode = QString("099");
    }
    return strCode;
}


void disposeDialog::startTool(QString code)
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    if(code=="002")
    {
        strPath = settings.value("onlinecheck/Linux.ExecuteFileName").toString();
    }
    else if(code=="010")
    {
        strPath = settings.value("onlinecheck/SQL.ExecuteFileName").toString();
    }
    else if(code=="011")
    {
        strPath = settings.value("onlinecheck/netWorkLeakScan.ExecuteFileName").toString();
    }
    else if(code=="012")
    {
        strPath = settings.value("onlinecheck/web.ExecuteFileName").toString();
    }
    else if(code=="013")
    {
        strPath = settings.value("onlinecheck/DB.ExecuteFileName").toString();
    }
    else
    {
        strPath=QString("nocode");
    }

    if(strPath!=QString("nocode")&&!strPath.isEmpty())
    {
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
}

bool disposeDialog::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    else
    {
       bool ok = dir.mkpath(fullPath);//创建多级目录
       return ok;
    }
}

void disposeDialog::refresh_tableWidget(QString id)
{
    for(int i=0;i<ui->attackTableWidget->rowCount();i++)
    {
        QString name = ui->attackTableWidget->item(i,2)->text();
        QString code = toolCode(name);
        if(code==id)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString("已完成"));
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->attackTableWidget->setItem(i,5,item);
            QSqlQuery query;
            QString strSql = QString("UPDATE check_task SET operate_state=2 WHERE id=%1 AND run_tool='%2'")
                    .arg(g_task_id).arg(name);
            bool ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
            }
            break;
        }
    }
}

QString disposeDialog::xmlPath(QString code)
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    if(code=="002")
    {
        strPath = settings.value("onlinecheck/Linux.InputFilePath").toString();
        strPath+=QString("\\%1.xml").arg(g_task_id);
    }
    else if(code=="011")
    {
        strPath = settings.value("onlinecheck/netWorkLeakScan.InputFilePath").toString();
        strPath+=QString("\\%1.xml").arg(g_task_id);
    }
    else if(code=="012")
    {
        strPath = settings.value("onlinecheck/web.InputFilePath").toString();
        strPath+=QString("\\%1.xml").arg(g_task_id);
    }
    else if(code=="013")
    {
        strPath = settings.value("onlinecheck/DB.InputFilePath").toString();
        strPath+=QString("\\%1.xml").arg(g_task_id);
    }
    else
    {
        strPath=QString("nocode");
    }
    return strPath;
}

//运行工具
void disposeDialog::on_runToolButton_clicked()
{
    if(attcount==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    row = ui->attackTableWidget->currentRow();
    if(row==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QString strState = ui->attackTableWidget->item(row,5)->text();
    if(strState==QString("已完成"))
    {
        QString strTitle = QString("提示");
        QString strMsg = QString("该任务已完成，是否查看结果？");
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
            connect(this,SIGNAL(checkReport()),this,SLOT(on_checkResButton_clicked()));
            emit checkReport();
            return;
        }
    }
    QString strIp = ui->attackTableWidget->item(row,1)->text();
    g_ip = strIp;
    QString strName = ui->attackTableWidget->item(row,2)->text();
    QString strType = ui->attackTableWidget->item(row,3)->text();
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"GBK\"");
    doc.appendChild(instruction);
    QDomElement config = doc.createElement("Config");
    doc.appendChild(config);
    QDomElement path = doc.createElement("ResultFilePath");

    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    QString strScanDir = settings.value("autoscan/ScanDir").toString();
    strScanDir+="\\";
    isDirExist(strScanDir);

    strPath = settings.value("autoscan/OnlineScanDir").toString();

    QString strId = QString("%1").arg(g_task_id);
    QString strCode = toolCode(strName);

    if(strCode!=QString("011"))
    {
        strPath+="\\";
    }

    //SQL注入验证工具不接受任务xml，所以直接启动
    if(strCode==QString("010"))
    {
        QString mPath = settings.value("onlinecheck/SQL.ExecuteFileName").toString();
        QString workPath = settings.value("onlinecheck/SQL.WorkPath").toString();
        bool ok = QProcess::startDetached(mPath,QStringList(),workPath);
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
        return;
    }

    g_code = strCode;
    //系统漏扫工具不识别带数字的任务XML。所以结果返回目录需要单独重写
    if(strCode==QString("011"))
    {
       isDirExist(strPath);
    }
    else
    {
        strPath+=strId;
        strPath+="\\";
        g_report_path = strPath;
        QString mStr = strIp;
        strPath+=mStr.remove(".");
        strPath+="\\";
        strPath+=strCode;
        isDirExist(strPath);
    }

    QDomText text_1 = doc.createTextNode(strPath);
    path.appendChild(text_1);
    config.appendChild(path);

    QDomElement cate = doc.createElement("toolcategory");
    QDomText text_2 = doc.createTextNode(strCode);
    cate.appendChild(text_2);
    config.appendChild(cate);

    QDomElement device = doc.createElement("device");
    config.appendChild(device);

    QDomElement code = doc.createElement("devicecode");
    QDomText text_3 = doc.createTextNode(strId);
    code.appendChild(text_3);
    device.appendChild(code);

    QDomElement ip = doc.createElement("dest_ip");
    QDomText text_4 = doc.createTextNode(strIp);
    ip.appendChild(text_4);
    device.appendChild(ip);

    QDomElement type = doc.createElement("devicetype");
    QDomText text_5 = doc.createTextNode(strType);
    type.appendChild(text_5);
    device.appendChild(type);

    QString filePath = xmlPath(strCode);
    if(filePath=="nocode")
    {
        QString strTitle = QString("提示");
        QString strMsg = QString("请插入U口工具");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        qWarning()<<"open file fail ";
        return;
    }
    QTextStream out(&file);
    if(strCode==QString("011"))
    {
        out.setCodec("GBK");
    }
    else
    {
        out.setCodec("UTF-8");
    }
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();

    startTool(strCode);

    QString action = QString("漏洞验证");
    QString state = QString("成功");
    operateLog(action,state);
    QTableWidgetItem *item = new QTableWidgetItem(QString("已下发"));
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->attackTableWidget->setItem(row,5,item);
    QSqlQuery query;
    QString strSql = QString("UPDATE check_task SET operate_state=1 WHERE id=%1 AND run_tool='%2' AND ip_address='%3'")
            .arg(g_task_id).arg(strName).arg(strIp);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
}

//删除文件夹
bool clearFolder(const QString &folderFullPath)
{

    QDir             dir(folderFullPath);
    QFileInfoList    fileList;
    QFileInfo        curFile;
    QFileInfoList    fileListTemp;
    int            infoNum;
    int            i;
    int            j;
    /* 首先获取目标文件夹内所有文件及文件夹信息 */
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                                |QDir::Readable|QDir::Writable
                                |QDir::Hidden|QDir::NoDotAndDotDot
                                ,QDir::Name);

    while(fileList.size() > 0)
    {
        infoNum = fileList.size();

        for(i = infoNum - 1; i >= 0; i--)
        {
            curFile = fileList[i];
            if(curFile.isFile()) /* 如果是文件，删除文件 */
            {
                QFile fileTemp(curFile.filePath());
                bool ok = fileTemp.remove();
                if(!ok)
                {
                    qWarning()<<"delete file err:"<<curFile.filePath();
                    return ok;
                }
                fileList.removeAt(i);
            }

            if(curFile.isDir()) /* 如果是文件夹 */
            {
                QDir dirTemp(curFile.filePath());
                fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
                                                    | QDir::Readable | QDir::Writable
                                                    | QDir::Hidden | QDir::NoDotAndDotDot
                                                    , QDir::Name);
                if(fileListTemp.size() == 0) /* 下层没有文件或文件夹 则直接删除*/
                {
                    dirTemp.rmdir(".");
                    fileList.removeAt(i);
                }
                else /* 下层有文件夹或文件 则将信息添加到列表*/
                {
                    for(j = 0; j < fileListTemp.size(); j++)
                    {
                        if(!(fileList.contains(fileListTemp[j])))
                        {
                            fileList.append(fileListTemp[j]);
                        }
                    }
                }
            }
        }
    }
    bool ifDel = dir.rmdir(".");/*删除目标文件夹,如果只是清空文件夹folderFullPath的内容而不删除folderFullPath本身,则删掉本行即可*/
    if(!ifDel)
    {
        qWarning()<<"delete file err:"<<dir.dirName();
        return false;
    }
    return true;
}

//删除资产
void disposeDialog::on_deleHostButton_clicked()
{
    if(attcount==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有可供删除的资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    int row = ui->attackTableWidget->currentRow();
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
    QString name = ui->attackTableWidget->item(row,0)->text();
    QString strIp = ui->attackTableWidget->item(row,1)->text();

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
        //删除结果文件夹和文件
        QString strPath;
        QString strTaskId = QString::number(g_task_id,10);
        QString strHostXml =  strTaskId+"_"+strIp; //匹配已解析xml的关键词
        QString localPath = qApp->applicationDirPath();
        localPath+="\\config.ini";
        QSettings settings(localPath,QSettings::IniFormat);
        strPath = settings.value("autoscan/OnlineScanDir").toString();
        strIp = strIp.remove(".");
        strPath = strPath+"\\"+strTaskId+"\\"+strIp;
        QFile existPath(strPath);
        if(existPath.exists())
        {
            bool ifSuccess = clearFolder(strPath);
            if(!ifSuccess)
            {
                QString strTitle = QString("警告");
                QString strMsg = QString("删除结果文件失败，请检查是否打开该资产的报告");
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
        //已经解析完的xml也要去删除
        QString mBakPath = settings.value("autoscan/comfilepath").toString();
        QStringList online_list;
        //获取所选文件类型过滤器
        QStringList filters;
        filters<<QString("*.xml");
        QDirIterator dir_iterator1(mBakPath,
            filters,
            QDir::Files | QDir::NoSymLinks,
            QDirIterator::Subdirectories);
        while(dir_iterator1.hasNext())
        {
            dir_iterator1.next();
            QFileInfo file_info1 =dir_iterator1.fileInfo();
            QString absolute_file_path1 =file_info1.absoluteFilePath();
            online_list.append(absolute_file_path1);
        }
        QString xmlFile;
        for(int i=0;i<online_list.size();i++)
        {
            xmlFile = online_list[i];
            int index = xmlFile.indexOf(strHostXml);
            if(index!=-1)
            {
                QFile deleFile(xmlFile);
                bool ok = deleFile.remove();
                if(!ok)
                {
                    qWarning()<<"delete xmlFile err:"<<xmlFile;
                    QString strTitle = QString("警告");
                    QString strMsg = QString("删除结果文件失败，请检查是否打开该资产的报告");
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

        mutex1.lock();
        if(g_ip_map.contains(strIp))
        {
            g_ip_map.remove(strIp);
        }
        mutex1.unlock();

        strSql = QString("SELECT * FROM check_task WHERE id = %1").arg(g_task_id);
        ok=query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<" sql:"<<strSql.toStdString().c_str();
        }
        int count_1=query.size();
        ui->attackTableWidget->setRowCount(count_1);
        int row_1=0;
        while(query.next())
        {
            QString name = query.value(1).toString();
            QString ip = query.value(2).toString();
            QString kind = query.value(3).toString();
            QString sys = query.value(6).toString();
            QString tool = query.value(7).toString();
            int state = query.value(8).toInt();
            QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1").arg(name));
            QTableWidgetItem *item2 = new QTableWidgetItem(QString("%1").arg(ip));
            QTableWidgetItem *item3 = new QTableWidgetItem(QString("%1").arg(kind));
            QTableWidgetItem *item4 = new QTableWidgetItem(QString("%1").arg(sys));
            QTableWidgetItem *item5 = new QTableWidgetItem(QString("%1").arg(tool));
            QTableWidgetItem *item6;
            if(state==0)
            {
                item6 = new QTableWidgetItem(QString("等待启动"));
            }
            else if(state==1)
            {
                item6 = new QTableWidgetItem(QString("已下发"));
            }
            else if(state==2)
            {
                item6 = new QTableWidgetItem(QString("已完成"));
            }
            item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->attackTableWidget->setItem(row_1,0,item1);
            ui->attackTableWidget->setItem(row_1,1,item2);
            ui->attackTableWidget->setItem(row_1,2,item5);
            ui->attackTableWidget->setItem(row_1,3,item3);
            ui->attackTableWidget->setItem(row_1,4,item4);
            ui->attackTableWidget->setItem(row_1,5,item6);
            ++row_1;
        }
        for(int i=0;i<count_1;i++)
        {
            ui->attackTableWidget->setRowHeight(i,40);
        }
        ui->attackTableWidget->setColumnWidth(1,120);
        ui->attackTableWidget->setColumnWidth(2,180);
        ui->attackTableWidget->setColumnWidth(3,150);
        ui->attackTableWidget->setColumnWidth(0,120);
        ui->attackTableWidget->setColumnWidth(4,110);
        ui->attackTableWidget->setColumnWidth(5,82);

        strSql = QString("SELECT * FROM host_info WHERE id = %1").arg(g_task_id);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error:"<<query.lastError()<<" sql:"<<strSql.toStdString().c_str();
        }
        int count_2 = query.size();
        ui->anaTableWidget->setRowCount(count_2);
        int row_2=0;
        while(query.next())
        {
            QString name = query.value(1).toString();
            QString ip = query.value(2).toString();
            QString kind = query.value(3).toString();
            QString sys = query.value(4).toString();
            QString state = QString("未关联");
            QTableWidgetItem *item1 = new QTableWidgetItem(QString("%1").arg(name));
            QTableWidgetItem *item2 = new QTableWidgetItem(QString("%1").arg(ip));
            QTableWidgetItem *item3 = new QTableWidgetItem(QString("%1").arg(kind));
            QTableWidgetItem *item4 = new QTableWidgetItem(QString("%1").arg(sys));
            QTableWidgetItem *item5 = new QTableWidgetItem(QString("%1").arg(state));
            item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->anaTableWidget->setItem(row_2,0,item1);
            ui->anaTableWidget->setItem(row_2,1,item2);
            ui->anaTableWidget->setItem(row_2,2,item3);
            ui->anaTableWidget->setItem(row_2,3,item4);
            ui->anaTableWidget->setItem(row_2,4,item5);
            ++row_2;
        }
        ui->anaTableWidget->setColumnWidth(0,150);
        ui->anaTableWidget->setColumnWidth(1,200);
        ui->anaTableWidget->setColumnWidth(2,170);
        ui->anaTableWidget->setColumnWidth(3,120);
        ui->anaTableWidget->setColumnWidth(4,126);

        QString action = QString("删除资产");
        QString state = QString("成功");
        operateLog(action,state);
    }
}

//处置报告确定按钮
void disposeDialog::on_recordButton_clicked()
{
    QSqlQuery query;
    QString strSql;

    QString strName =  ui->webTableWidget->item(0,1)->text();
    QString strIp =  ui->webTableWidget->item(0,3)->text();
    QString strWeb =  ui->webTableWidget->item(1,1)->text();
    QString strCom =  ui->webTableWidget->item(2,1)->text();
    QString strCom2 =  ui->webTableWidget->item(2,3)->text();
    QString strPerson =  ui->webTableWidget->item(3,1)->text();
    QString strPhone =  ui->webTableWidget->item(3,3)->text();
    QString strPerson2 =  ui->webTableWidget->item(4,1)->text();
    QString strPhone2 =  ui->webTableWidget->item(4,3)->text();
    QString strPlace =  ui->webTableWidget->item(5,1)->text();
    QString strIcp =  ui->webTableWidget->item(6,1)->text();
    QString strInternet =  ui->webTableWidget->item(6,3)->text();
    QString strConnect =  ui->webTableWidget->item(7,1)->text();
    QString strComKind =  ui->webTableWidget->item(7,3)->text();
    QString strKind =  ui->webTableWidget->item(8,1)->text();
    QString strLevel =  ui->webTableWidget->item(8,3)->text();
    QString strLevel2 =  ui->webTableWidget->item(9,1)->text();
    QString strSafe =  ui->webTableWidget->item(9,3)->text();
    QString strService =  ui->webTableWidget->item(10,1)->text();

    QString strLanguage =  ui->archTableWidget->item(0,1)->text();
    QString strPingtai =  ui->archTableWidget->item(0,3)->text();
    QString strMiddle =  ui->archTableWidget->item(1,1)->text();
    QString strDatabase =  ui->archTableWidget->item(2,1)->text();
    QString strServer =  ui->archTableWidget->item(3,1)->text();
    QString strWeihu =  ui->archTableWidget->item(4,1)->text();

    QString str_1 =  ui->antiTableWidget->item(0,1)->text();
    QString str_2 =  ui->antiTableWidget->item(1,1)->text();
    QString str_3 =  ui->antiTableWidget->item(2,1)->text();
    QString str_4 =  ui->antiTableWidget->item(3,1)->text();
    QString str_5 =  ui->antiTableWidget->item(4,1)->text();
    QString str_6 =  ui->antiTableWidget->item(5,1)->text();
    QString str_7 =  ui->antiTableWidget->item(6,1)->text();
    QString str_8 =  ui->antiTableWidget->item(7,1)->text();
    QString str_9 =  ui->antiTableWidget->item(8,1)->text();
    QString str_10 =  ui->antiTableWidget->item(9,1)->text();

    strSql = QString("UPDATE web_record_form SET web_ch_name='%1',ip_address='%2',website='%3',"
                                    "responsibility_com='%4',run_company='%5',principal_1='%6',phone_1='%7',"
                                    "principal_2='%8',phone_2='%9',res_com_addr='%10',icp_filing='%11',"
                                    "internet_filing='%12',affiliation='%13',com_type='%14',industry_type='%15',"
                                    "protect_filing='%16',evaluation='%17',security_responsibility='%18',"
                                    "service_seclion='%19',development_language='%20',build_platform='%21',"
                                    "middle_ware='%22',datebase='%23',server='%24',remote_maintain='%25',"
                                    "anti_attack='%26',anti_tamper='%27',anti_virus='%28',anti_palsy='%29',"
                                    "safe_audit='%30',perioheral_threat='%31',plugin_description='%32',"
                                    "web_manage='%33',safe_detect='%34',safe_protect='%35' WHERE id=%36")
            .arg(strName).arg(strIp).arg(strWeb).arg(strCom).arg(strCom2).arg(strPerson).arg(strPhone)
            .arg(strPerson2).arg(strPhone2).arg(strPlace).arg(strIcp).arg(strInternet).arg(strConnect)
            .arg(strComKind).arg(strKind).arg(strLevel).arg(strLevel2).arg(strSafe).arg(strService).arg(strLanguage)
            .arg(strPingtai).arg(strMiddle).arg(strDatabase).arg(strServer).arg(strWeihu).arg(str_1).arg(str_2)
            .arg(str_3).arg(str_4).arg(str_5).arg(str_6).arg(str_7).arg(str_8).arg(str_9).arg(str_10).arg(g_task_id);

    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is "<<strSql.toStdString().c_str();
    }
    QString strTitle = QString("成功");
    QString strMsg = QString("保存成功");
    QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
    if(NULL!=WrrMsg->button(QMessageBox::Ok))
    {
        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
    }
    WrrMsg->exec();
}

void disposeDialog::on_recordButton_2_clicked()
{
    QString strName = ui->safeTableWidget->item(1,1)->text();
    QString strContact = ui->safeTableWidget->item(1,3)->text();
    QString strSystem = ui->safeTableWidget->item(2,1)->text();
    QString strLevel = ui->safeTableWidget->item(2,3)->text();
    QString strWeb = ui->safeTableWidget->item(3,1)->text();
    QString strIp = ui->safeTableWidget->item(3,3)->text();
    QString strService = ui->safeTableWidget->item(4,1)->text();
    QString strData = ui->safeTableWidget->item(4,3)->text();
    QString strOperate = ui->safeTableWidget->item(5,1)->text();
    QString strServer = ui->safeTableWidget->item(5,3)->text();
    QString strData2 = ui->safeTableWidget->item(6,1)->text();
    QString strThird = ui->safeTableWidget->item(6,3)->text();
    QString strTech = ui->safeTableWidget->item(7,1)->text();

    char strTime1[128]={ 0 };
    time_t time = date_1->dateTime().toTime_t();
    GetAsciiTime(time,strTime1,128);
    QString strOri = ui->safeTableWidget->item(9,3)->text();
    QString strAtt = ui->safeTableWidget->item(10,1)->text();
    char strTime2[128]={ 0 };
    time = date_2->dateTime().toTime_t();
    GetAsciiTime(time,strTime2,128);

    QString strDamage = ui->safeTableWidget->item(11,1)->text();
    QString strEffect = ui->safeTableWidget->item(12,1)->text();
    QString strTimeAndPlace = ui->safeTableWidget->item(12,3)->text();
    QString strDisCom = ui->safeTableWidget->item(13,1)->text();
    QString strDisPer = ui->safeTableWidget->item(13,3)->text();

    QSqlQuery query;
    QString strSql = QString("UPDATE safe_record_form SET company_name='%1',contact_person='%2',"
                                            "system_name='%3',filing_level='%4',website_domain='%5',ip_address='%6',"
                                            "service_content='%7',datebase_content_num='%8',operating_name_version='%9',"
                                            "server_name='%10',datebase_name='%11',third_party_app='%12'"
                                            ",protect_equioment='%13',discovery_time='%14',"
                                            "discovery_source='%15',damage_degree='%16',attack_time='%17',sys_damage='%18',"
                             "user_effect='%19',dis_timeplace='%20',dis_company='%21',dis_person='%22' WHERE id=%23")
            .arg(strName).arg(strContact).arg(strSystem).arg(strLevel).arg(strWeb).arg(strIp).arg(strService).arg(strData)
            .arg(strOperate).arg(strServer).arg(strData2).arg(strThird).arg(strTech).arg(strTime1).arg(strOri).arg(strAtt)
            .arg(strTime2).arg(strDamage).arg(strEffect).arg(strTimeAndPlace).arg(strDisCom).arg(strDisPer).arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
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
        QString strMsg = QString("保存成功");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        safeTableWidgetShow();
    }
}



void disposeDialog::on_saveButton_clicked()
{
    QString strHole = QString("");
    if(ui->holeCheckBox_1->isChecked())
    {
        strHole+=QString("远程溢出,");
    }
    if(ui->holeCheckBox_2->isChecked())
    {
        strHole+=QString("SQL注入,");
    }
    if(ui->holeCheckBox_3->isChecked())
    {
        strHole+=QString("后台管理验证绕过,");
    }
    if(ui->holeCheckBox_4->isChecked())
    {
        strHole+=QString("暗链,");
    }
    if(ui->holeCheckBox_5->isChecked())
    {
        strHole+=QString("恶意修改,");
    }
    if(ui->holeCheckBox_6->isChecked())
    {
        strHole+=QString("跨站脚本,");
    }
    if(ui->holeCheckBox_7->isChecked())
    {
        strHole+=QString("攻击痕迹检测,");
    }
    if(ui->holeCheckBox_8->isChecked())
    {
        strHole+=QString("木马,");
    }
    if(ui->holeCheckBox_9->isChecked())
    {
        strHole+=QString("CVE风险漏洞,");
    }
    if(ui->holeCheckBox_10->isChecked())
    {
        strHole+=QString("文件上传,");
    }
    if(ui->holeCheckBox_11->isChecked())
    {
        strHole+=QString("信息泄露,");
    }
    if(ui->holeCheckBox_12->isChecked())
    {
        strHole+=QString("危险端口开放,");
    }
    if(ui->holeCheckBox_13->isChecked())
    {
        strHole+=QString("目录遍历,");
    }
    if(ui->holeCheckBox_14->isChecked())
    {
        strHole+=QString("冗余文件,");
    }
    if(ui->holeCheckBox_15->isChecked())
    {
        strHole+=QString("默认配置文件");
    }
    QString strProcess = edit->toPlainText();
    QString strReason = edit_2->toPlainText();
    QString strResult = edit_3->toPlainText();
    QString strOther = ui->otherTextEdit->toPlainText();

    QSqlQuery query;
    QString strSql = QString("SELECT * FROM website_condition WHERE id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    int count = query.size();
    if(count==0)
    {
        strSql = QString("INSERT INTO website_condition(id,loopholes,process,reason,result,other) VALUES"
                                                "(%1,'%2','%3','%4','%5','%6')").arg(g_task_id).arg(strHole).arg(strProcess)
                                                .arg(strReason).arg(strResult).arg(strOther);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
        }
        QString strTitle = QString("成功");
        QString strMsg = QString("保存成功");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }
    else if(count==1)
    {
        strSql = QString("UPDATE website_condition SET loopholes='%1',process='%2',reason='%3'"
                                        ",result='%4',other='%5' WHERE id=%6").arg(strHole).arg(strProcess)
                                        .arg(strReason).arg(strResult).arg(strOther).arg(g_task_id);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
        }
        QString strTitle = QString("成功");
        QString strMsg = QString("保存成功");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }
    else if(count==2)
    {
        qWarning()<<"database error!";
        QString strTitle = QString("出错");
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

void disposeDialog::disposeShow()
{
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM website_condition WHERE id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    QString strHole,strProcess,strReason,strResult,strOther;
    if(query.next())
    {
        strHole = query.value(1).toString();
        strProcess = query.value(2).toString();
        strReason = query.value(3).toString();
        strResult = query.value(4).toString();
        strOther = query.value(5).toString();
        QStringList holeList = strHole.split(",");
        for(int i=0;i<holeList.size();i++)
        {
            if(holeList[i]==QString("远程溢出"))
            {
                ui->holeCheckBox_1->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("SQL注入"))
            {
                ui->holeCheckBox_2->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("后台管理验证绕过"))
            {
                ui->holeCheckBox_3->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("暗链"))
            {
                ui->holeCheckBox_4->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("恶意修改"))
            {
                ui->holeCheckBox_5->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("跨站脚本"))
            {
                ui->holeCheckBox_6->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("攻击痕迹检测"))
            {
                ui->holeCheckBox_7->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("木马"))
            {
                ui->holeCheckBox_8->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("CVE风险漏洞"))
            {
                ui->holeCheckBox_9->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("文件上传"))
            {
                ui->holeCheckBox_10->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("信息泄露"))
            {
                ui->holeCheckBox_11->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("危险端口开放"))
            {
                ui->holeCheckBox_12->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("目录遍历"))
            {
                ui->holeCheckBox_13->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("冗余文件"))
            {
                ui->holeCheckBox_14->setCheckState(Qt::Checked);
            }
            if(holeList[i]==QString("默认配置文件"))
            {
                ui->holeCheckBox_15->setCheckState(Qt::Checked);
            }
        }
        edit->setText(strProcess);
        edit_2->setText(strReason);
        edit_3->setText(strResult);
        ui->otherTextEdit->setText(strOther);
    }
}


//解析系统漏扫结果XML
bool ifComplete(QString ip,QString path,QString &reportPath)
{
    QDir dir(path);
    if(dir.exists())
    {
        QString strTaskId = QString::number(g_task_id,10);
        QStringList filters;
        filters<<QString("*.xml");
        dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
        dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
        int dir_count = dir.count();
        if(dir_count > 0)
        {
            QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir.entryInfoList(filters));
            QDomDocument doc;
            for(int i=0;i<dir_count;i++)
            {
                QString fileName = fileInfo->at(i).absoluteFilePath();
                QFile file3(fileName);
                if (!file3.open(QFile::ReadOnly | QFile::Text))
                {
                    qWarning()<<"open result xml err:"<<fileName;
                    continue;
                }
                doc.setContent(&file3);
                file3.close();
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch=docElem.elementsByTagName("task_info");
                QDomNode node = nodeswitch.item(0);
                QDomElement elemnodeswitch=node.toElement();
                QString taskCode = qPrintable(elemnodeswitch.attribute("taskcode"));

                QDomNodeList nodeswitch_2=docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement elemnodeswitch_2=node_2.toElement();
                QString destIp = qPrintable(elemnodeswitch_2.attribute("dest_ip"));

                if(taskCode==strTaskId)
                {
                    QStringList ipList = destIp.split(",");
                    int count = ipList.size();
                    for(int i=0;i<count;i++)
                    {
                        QString mIp = ipList[i];
                        if(mIp==ip)
                        {
                            reportPath = fileName;
                            return true;
                        }
                    }
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}

//查看结果
void disposeDialog::on_checkResButton_clicked()
{
    ui->checkResButton->setEnabled(false);
    if(attcount==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->checkResButton->setEnabled(true);
        return;
    }
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    QString mPath = settings.value("autoscan/OnlineScanDir").toString();

    int mRow = ui->attackTableWidget->currentRow();
    if(mRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->checkResButton->setEnabled(true);
        return;
    }
    QString strState = ui->attackTableWidget->item(mRow,5)->text();
    if(strState==QString("等待启动"))
    {
        QString strTitle = QString("提示");
        QString strMsg = QString("请启动检查工具进行检查");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->checkResButton->setEnabled(true);
        return;
    }
    QString strIp = ui->attackTableWidget->item(mRow,1)->text();
    QString strName = ui->attackTableWidget->item(mRow,2)->text();

    QString strId = QString("%1").arg(g_task_id);
    QString strCode = toolCode(strName);
    if(strCode=="002"||strCode=="010"||strCode=="011"||strCode=="012"||strCode=="013")
    {
        mPath+="\\";
        mPath+=strId;
        mPath+="\\";
        QString mStr = strIp;
        mPath+=mStr.remove(".");
        mPath+="\\";
        mPath+=strCode;
        isDirExist(mPath);
        QString mReportPath;      //存放系统漏扫结果docx的目录
        QString reportPath = mPath+"\\result.docx";
        QFileInfo  file1(reportPath);
        if(file1.exists())
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(reportPath).absoluteFilePath()));
        }
        else
        {
            //判断路径是否存在
            QDir dir(mPath);
            if(!dir.exists())
            {
                qWarning()<<"result xml path error!";
                QString strTitle = QString("错误");
                QString strMsg = QString("结果文件夹不存在！请检查是否误删除");
                QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                if(NULL!=WrrMsg->button(QMessageBox::Ok))
                {
                    WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                }
                WrrMsg->exec();
                ui->checkResButton->setEnabled(true);
                return;
            }

            if(strCode!="011")
            {
                QStringList filters;
                filters<<QString("*.xml");
                dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
                dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
                int dir_count = dir.count();
                if(dir_count <= 0)
                {
                    QString strTitle = QString("警告");
                    QString strMsg = QString("任务还在扫描");
                    QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                    if(NULL!=WrrMsg->button(QMessageBox::Ok))
                    {
                        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                    }
                    WrrMsg->exec();
                    ui->checkResButton->setEnabled(true);
                    return;
                }
            }
            else if(strCode=="011")
            {
                QString sysPath = settings.value("autoscan/OnlineScanDir").toString();
                sysPath += "\\";
                bool ifCom = ifComplete(strIp,sysPath,mReportPath);
                if(!ifCom)
                {
                    QString strTitle = QString("警告");
                    QString strMsg = QString("任务还在扫描");
                    QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                    if(NULL!=WrrMsg->button(QMessageBox::Ok))
                    {
                        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                    }
                    WrrMsg->exec();
                    ui->checkResButton->setEnabled(true);
                    return;
                }
            }
            //获取分隔符
            QChar separator = QChar('/');
            if(!mPath.contains(separator))
            {
                separator = QChar('\\');
            }
            QChar last_char = mPath.at(mPath.length()-1);
            if(last_char == separator)
            {
                separator = QChar();
            }
            QString file_name = dir[0];  //文件名称
            QString file_path;
            if(strCode!="011")
            {
                file_path = mPath + separator + file_name;   //文件全路径
            }
            else
            {
                file_path = mReportPath;
            }            
            typedef int (*Fun)(char*, char*, char*, int);
            QLibrary mylib("ToolXmlToWord.dll");
            if(mylib.load())
            {
                Fun xmlToWord = (Fun)mylib.resolve("XmlToWord");
                if(xmlToWord)
                {
                    QString tpFilePath = qApp->applicationDirPath();
                    tpFilePath += QString("\\模板文件\\");
                    tpFilePath += strName;
                    tpFilePath += ".docx";
                    char* strTpPath;
                    QByteArray by = tpFilePath.toLocal8Bit();
                    strTpPath = by.data();
                    char* strReportPath;
                    QByteArray by2 = reportPath.toLocal8Bit();
                    strReportPath = by2.data();
                    char* strXmlPath;
                    QByteArray by3 = file_path.toLocal8Bit();
                    strXmlPath = by3.data();
                    int intTool = strCode.toInt();
                    int i=xmlToWord(strTpPath,strXmlPath,strReportPath,intTool);
                    if(i==0)
                    {
                        QString localPath = qApp->applicationDirPath();
                        localPath+="\\config.ini";
                        QSettings settings(localPath,QSettings::IniFormat);
                        QString strBak = settings.value("autoscan/comfilepath").toString();
                        isDirExist(strBak);
                        QString strTaskId = QString::number(g_task_id,10);
                        QString bakPath = strBak+"\\"+strTaskId+"_"+strIp+"_"+strCode+".xml";
                        QFile pLogFile(bakPath);
                        if(pLogFile.exists())
                        {
                            QFile::remove(bakPath);         //只保留一个
                        }
                        QFile::copy(file_path, bakPath); // Bug: 按理说 rename 会更合适，但是 rename 时最后一个文件总是显示不出来，需要 killall Finder 后才出现
                        QFile::remove(file_path); // 删除
                        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(strReportPath).absoluteFilePath()));
                    }
                    else
                    {
                        QString strTitle = QString("错误");
                        QString strMsg = QString("生成报告失败");
                        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                        if(NULL!=WrrMsg->button(QMessageBox::Ok))
                        {
                            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                        }
                        WrrMsg->exec();
                        ui->checkResButton->setEnabled(true);
                        return;
                    }
                }
            }
        }
    }
    else
    {
        QString mPath = settings.value("autoscan/ScanDir").toString();
        QString resPath = mPath+"\\"+strCode;
        resPath+=strIp;
        resPath+="res.docx";
        QFileInfo  file2(resPath);
        bool flag = false;
        if(file2.exists())
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(resPath));
            ui->checkResButton->setEnabled(true);
            return;
        }
        else
        {
            QDir dir(mPath);
            if(!dir.exists())
            {
                isDirExist(mPath);
            }

            QStringList filters;
            filters<<QString("*.xml");
            dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
            dir.setNameFilters(filters);

            int dir_count = dir.count();
            if(dir_count <= 0)
            {
                QString strTitle = QString("警告");
                QString strMsg = QString("任务还在扫描");
                QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                if(NULL!=WrrMsg->button(QMessageBox::Ok))
                {
                    WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                }
                WrrMsg->exec();
                ui->checkResButton->setEnabled(true);
                return;
            }

            QStringList file_list;
            //获取分隔符
            QChar separator = QChar('/');
            if(!mPath.contains(separator))
            {
                separator = QChar('\\');
            }
            QChar last_char = mPath.at(mPath.length()-1);
            if(last_char == separator)
            {
                separator = QChar();
            }

            for(int i=0;i<dir_count;i++)
            {
                QString file_name = dir[i];  //文件名称
                QString file_path = mPath + separator + file_name;   //文件全路径
                file_list.append(file_path);
            }

            QDomDocument doc;
            for(int i=0;i<file_list.size();i++)
            {
                QString xmlPath = file_list[i];
                QFile file3(xmlPath);
                if (!file3.open(QFile::ReadOnly | QFile::Text))
                {
                    QString strTitle = QString("警告");
                    QString strMsg = QString("打开结果XML失败");
                    QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                    if(NULL!=WrrMsg->button(QMessageBox::Ok))
                    {
                        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                    }
                    WrrMsg->exec();
                    ui->checkResButton->setEnabled(true);
                    return;
                }
                doc.setContent(&file3);
                file3.close();
                QDomElement docElem = doc.documentElement();
                QDomNodeList nodeswitch=docElem.elementsByTagName("tool_info");
                QDomNode node = nodeswitch.item(0);
                QDomElement elemnodeswitch=node.toElement();
                QString toolCode = qPrintable(elemnodeswitch.attribute("toolcategory"));

                QDomNodeList nodeswitch_2=docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement elemnodeswitch_2=node_2.toElement();
                QString destIp = qPrintable(elemnodeswitch_2.attribute("dest_ip"));

                if(toolCode==strCode)
                {
                    QStringList ip_list = destIp.split(";");
                    for(int i=0;i<ip_list.size();i++)
                    {
                        if(ip_list[i]==strIp)
                        {
                            typedef int (*Fun)(char*, char*, char*, int);
                            QLibrary mylib("ToolXmlToWord.dll");
                            if(mylib.load())
                            {
                                Fun xmlToWord = (Fun)mylib.resolve("XmlToWord");
                                if(xmlToWord)
                                {
                                    QString tpFilePath = qApp->applicationDirPath();
                                    tpFilePath += QString("\\模板文件\\");
                                    tpFilePath += strName;
                                    tpFilePath += ".docx";
                                    char* strTpPath;
                                    QByteArray by = tpFilePath.toLocal8Bit();
                                    strTpPath = by.data();
                                    char* strReportPath;
                                    QByteArray by2 = resPath.toLocal8Bit();
                                    strReportPath = by2.data();
                                    char* strXmlPath;
                                    QByteArray by3 = xmlPath.toLocal8Bit();
                                    strXmlPath = by3.data();
                                    int intTool = strCode.toInt();
                                    int i=xmlToWord(strTpPath,strXmlPath,strReportPath,intTool);
                                    if(i==0)
                                    {
                                        QString localPath = qApp->applicationDirPath();
                                        localPath+="\\config.ini";
                                        QSettings settings(localPath,QSettings::IniFormat);
                                        QString strBak = settings.value("autoscan/comfilepath").toString();
                                        isDirExist(strBak);
                                        QString strTaskId = QString::number(g_task_id,10);
                                        QString bakPath = strBak+"\\"+strTaskId+"_"+strIp+"_"+strCode+".xml";
                                        QFile pLogFile(bakPath);
                                        if(pLogFile.exists())
                                        {
                                            QFile::remove(bakPath);         //只保留一个
                                        }
                                        QFile::copy(xmlPath, bakPath); // Bug: 按理说 rename 会更合适，但是 rename 时最后一个文件总是显示不出来，需要 killall Finder 后才出现
                                        QFile::remove(xmlPath); // 删除
                                        QDesktopServices::openUrl(QUrl::fromLocalFile(resPath));
                                    }
                                    else
                                    {
                                        QString strTitle = QString("错误");
                                        QString strMsg = QString("生成报告失败");
                                        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                                        if(NULL!=WrrMsg->button(QMessageBox::Ok))
                                        {
                                            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                                        }
                                        WrrMsg->exec();
                                        ui->checkResButton->setEnabled(true);
                                        return;
                                    }
                                }
                            }
                           flag = true;
                        }
                    }
                }
            }
            if(flag==false)
            {
                QString strTitle = QString("提示");
                QString strMsg = QString("未找到符合的xml");
                QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                if(NULL!=WrrMsg->button(QMessageBox::Ok))
                {
                    WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                }
                WrrMsg->exec();
                QString fileName = QFileDialog::getOpenFileName(this,
                                                                "Open File",
                                                                mPath,
                                                                "ReportFile(*.xml)");
                if(fileName.isEmpty())
                {
                    ui->checkResButton->setEnabled(true);
                    return;
                }
                typedef int (*Fun)(char*, char*, char*, int);
                QLibrary mylib("ToolXmlToWord.dll");
                if(mylib.load())
                {
                    Fun xmlToWord = (Fun)mylib.resolve("XmlToWord");
                    if(xmlToWord)
                    {
                        QString tpFilePath = qApp->applicationDirPath();
                        tpFilePath += QString("\\模板文件\\");
                        tpFilePath += strName;
                        tpFilePath += ".docx";
                        char* strTpPath;
                        QByteArray by = tpFilePath.toLocal8Bit();
                        strTpPath = by.data();
                        char* strReportPath;
                        QByteArray by2 = resPath.toLocal8Bit();
                        strReportPath = by2.data();
                        char* strXmlPath;
                        QString mXmlPath = QFileInfo(fileName).absoluteFilePath();
                        QByteArray by3 = mXmlPath.toLocal8Bit();
                        strXmlPath = by3.data();
                        int intTool = strCode.toInt();
                        int i=xmlToWord(strTpPath,strXmlPath,strReportPath,intTool);
                        if(i==0)
                        {
                            QDesktopServices::openUrl(QUrl::fromLocalFile(resPath));
                        }
                        else
                        {
                            QString strTitle = QString("错误");
                            QString strMsg = QString("生成报告失败");
                            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
                            if(NULL!=WrrMsg->button(QMessageBox::Ok))
                            {
                                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                            }
                            WrrMsg->exec();
                            ui->checkResButton->setEnabled(true);
                            return;
                        }
                    }
                }
            }
        }
    }
    ui->checkResButton->setEnabled(true);
}

void disposeDialog::on_analyzeButton_clicked()
{
    int row = ui->anaTableWidget->currentRow();
    if(row==-1)
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请选择资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QString hostName = ui->anaTableWidget->item(row,0)->text();
    QString hostIp = ui->anaTableWidget->item(row,1)->text();
    QSqlQuery query;
    QString strSql = QString("UPDATE host_info SET state='已在平台分析' WHERE host_name='%1' AND "
                             "ip_address='%2'").arg(hostName).arg(hostIp);
    bool ok=query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err:"<<query.lastError();
    }
    QDesktopServices::openUrl(QUrl("https://172.16.39.88:8443"));
    QTableWidgetItem *item = new QTableWidgetItem("已在平台分析");
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->anaTableWidget->setItem(row,4,item);
}

DWORD GetProcessidFromName(QString name)
{
    std::string str_1 = name.toStdString();
    const char* strName = str_1.c_str();
    PROCESSENTRY32 pe;
    DWORD id=0;
    HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    pe.dwSize=sizeof(PROCESSENTRY32);
    if(!Process32First(hSnapshot,&pe))
        return 0;
    while(1)
    {
        pe.dwSize=sizeof(PROCESSENTRY32);
        if(Process32Next(hSnapshot,&pe)==FALSE)
            break;
        //WChar*转Char*
        size_t len = wcslen(pe.szExeFile) + 1;
        size_t converted = 0;
        char *CStr;
        CStr=(char*)malloc(len*sizeof(char));
        wcstombs_s(&converted, CStr, len, pe.szExeFile, _TRUNCATE);
        if(strcmp(CStr,strName)==0)
        {
            id=pe.th32ProcessID;
            delete CStr;
            CStr = NULL;
            break;
        }
        delete CStr;
        CStr = NULL;
    }
    CloseHandle(hSnapshot);
    return id;
}

QString getExeName(QString exePath)
{
    QStringList pathList = exePath.split("\\");
    int count = pathList.size();
    QString exeName = pathList[count-1];
    return exeName;
}

void disposeDialog::checkToolStart()
{
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    QString path_1 = settings.value("onlinecheck/netWorkLeakScan.InputFilePath").toString();  //系统漏扫
    QString path_2 = settings.value("onlinecheck/web.InputFilePath").toString();              //webscan
    QString path_3 = settings.value("onlinecheck/DB.InputFilePath").toString();               //数据库
    QString path_4 = settings.value("onlinecheck/Linux.InputFilePath").toString();            //Linux

    QString tool_1 = settings.value("onlinecheck/netWorkLeakScan.ExecuteFileName").toString();
    QString tool_2 = settings.value("onlinecheck/web.ExecuteFileName").toString();
    QString tool_3 = settings.value("onlinecheck/DB.ExecuteFileName").toString();
    QString tool_4 = settings.value("onlinecheck/Linux.ExecuteFileName").toString();

    QString exeName;

    int dir_count = 0;
    //判断路径是否存在
    QDir dir_1(path_1);
    if(!dir_1.exists())
    {
        qWarning()<<"netWorkLeakScan task xml path is deleted!";
        return;
    }
    QStringList filters;
    filters<<QString("*.xml");
    dir_1.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir_1.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
    dir_count = dir_1.count();
    if(dir_count > 0)
    {
        exeName = getExeName(tool_1);
        if(GetProcessidFromName(exeName)==0)
        {
            bool ok = QProcess::startDetached(tool_1,QStringList());
            if(!ok)
            {
//                qWarning()<<"start tool failed!";
            }
        }
    }

    QDir dir_2(path_2);
    if(!dir_2.exists())
    {
        qWarning()<<"webscan task xml path is deleted!";
        return;
    }
    dir_2.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir_2.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
    dir_count = dir_2.count();
    if(dir_count > 0)
    {
        exeName = getExeName(tool_2);
        if(GetProcessidFromName(exeName)==0)
        {
            bool ok = QProcess::startDetached(tool_2,QStringList());
            if(!ok)
            {
//                qWarning()<<"start tool failed!";
            }
        }
    }

    QDir dir_3(path_3);
    if(!dir_3.exists())
    {
        qWarning()<<"DB task xml path is deleted!";
        return;
    }
    dir_3.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir_3.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
    dir_count = dir_3.count();
    if(dir_count > 0)
    {
        exeName = getExeName(tool_3);
        if(GetProcessidFromName(exeName)==0)
        {
            bool ok = QProcess::startDetached(tool_3,QStringList());
            if(!ok)
            {
//                qWarning()<<"start tool failed!";
            }
        }
    }

    QDir dir_4(path_4);
    if(!dir_4.exists())
    {
        qWarning()<<"netWorkLeakScan task xml path is deleted!";
        return;
    }
    dir_4.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir_4.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
    dir_count = dir_4.count();
    if(dir_count > 0)
    {
        exeName = getExeName(tool_4);
        if(GetProcessidFromName(exeName)==0)
        {
            bool ok = QProcess::startDetached(tool_4,QStringList());
            if(!ok)
            {
//                qWarning()<<"start tool failed!";
            }
        }
    }
}

//手动关联
void disposeDialog::on_chooseButton_clicked()
{
    int mRow = ui->attackTableWidget->currentRow();
    if(mRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->checkResButton->setEnabled(true);
        return;
    }
    QString strState = ui->attackTableWidget->item(mRow,5)->text();
    if(strState==QString("已关联"))
    {
        QString strTitle = QString("提示");
        QString strMsg = QString("该资产已关联，是否重新关联？");
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
        }
    }
    QString strHost = ui->attackTableWidget->item(mRow,0)->text();
    QString strIp = ui->attackTableWidget->item(mRow,1)->text();
    QString strName = ui->attackTableWidget->item(mRow,2)->text();

//    QString strId = QString("%1").arg(g_task_id);
    QString strCode = toolCode(strName);
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    QString mPath = settings.value("autoscan/ScanDir").toString();
    QString resPath = mPath+"\\"+strCode;
    resPath+=strIp;
    resPath+="res.docx";

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open File",
                                                    mPath,
                                                    "ReportFile(*.xml)");
    if(fileName.isEmpty())
    {
        return;
    }

    QDomDocument doc;
    QFile file3(fileName);
    if (!file3.open(QFile::ReadOnly | QFile::Text))
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("打开结果XML失败");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    doc.setContent(&file3);
    file3.close();

    QDomElement docElem = doc.documentElement();
    QDomNodeList nodeswitch=docElem.elementsByTagName("tool_info");
    QDomNode node = nodeswitch.item(0);
    QDomElement elemnodeswitch=node.toElement();
    QString toolCode = qPrintable(elemnodeswitch.attribute("toolcategory"));

    QDomNodeList nodeswitch_2=docElem.elementsByTagName("operation_info");
    QDomNode node_2 = nodeswitch_2.item(0);
    QDomElement elemnodeswitch_2=node_2.toElement();
    QString destIp = qPrintable(elemnodeswitch_2.attribute("src_ip"));
    destIp = destIp+strIp+";";
    elemnodeswitch_2.setAttribute("src_ip",destIp);

    QString strTaskId = QString::number(g_task_id,10);

    QDomNodeList node_list = docElem.elementsByTagName("scheme");
    QDomNode schNode = node_list.item(0);
    QDomNodeList schNode_list = schNode.childNodes();
    QDomNode oldNode = schNode_list.at(1);
    QDomElement oldEle = oldNode.toElement();
    oldEle.setAttribute("taskcode",strTaskId);
    QDomNode newnode = schNode_list.at(1);
    schNode.replaceChild(newnode,oldNode);

    if(!file3.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file3);
    doc.save(out_stream,4); //缩进4格
    file3.close();

    if(toolCode==strCode)
    {
    }
    else
    {
        QString strTitle = QString("关联失败");
        QString strMsg = QString("请选择正确的结果XML");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }

    typedef int (*Fun)(char*, char*, char*, int);
    QLibrary mylib("ToolXmlToWord.dll");
    if(mylib.load())
    {
        Fun xmlToWord = (Fun)mylib.resolve("XmlToWord");
        if(xmlToWord)
        {
            QString tpFilePath = qApp->applicationDirPath();
            tpFilePath += QString("\\模板文件\\");
            tpFilePath += strName;
            tpFilePath += ".docx";
            char* strTpPath;
            QByteArray by = tpFilePath.toLocal8Bit();
            strTpPath = by.data();
            char* strReportPath;
            QByteArray by2 = resPath.toLocal8Bit();
            strReportPath = by2.data();
            char* strXmlPath;
            QString mXmlPath = QFileInfo(fileName).absoluteFilePath();
            QByteArray by3 = mXmlPath.toLocal8Bit();
            strXmlPath = by3.data();
            int intTool = strCode.toInt();
            int i=xmlToWord(strTpPath,strXmlPath,strReportPath,intTool);
            if(i==0)
            {
                QTableWidgetItem *item = new QTableWidgetItem(QString("已关联"));
                item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                ui->attackTableWidget->setItem(mRow,5,item);
                QString strSql = QString("UPDATE check_task SET operate_state=3 WHERE host_name='%1' "
                                                        "AND run_tool='%2'").arg(strHost).arg(strName);
                QSqlQuery query;
                query.exec(strSql);

                QString strTitle = QString("关联成功");
                QString strMsg = QString("是否打开生成的word文档？");
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

                if(ok == QMessageBox::Ok)
                {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(resPath));
                }
                else
                {
                    return;
                }
            }
            else
            {
                QString strTitle = QString("错误");
                QString strMsg = QString("生成报告失败");
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
    else
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("加载word模块失败");
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

void disposeDialog::checkResultSlot()
{
    QString strSql;
    QSqlQuery query;
    strSql = QString("SELECT * FROM check_task WHERE id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err:"<<query.lastError();
//        QString strTitle = QString("错误");
//        QString strMsg = QString("数据库错误");
//        QString showMsg = "<font color='black'>"+strMsg+"</font>";
//        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
//        if(NULL!=WrrMsg->button(QMessageBox::Ok))
//        {
//            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
//        }
//        WrrMsg->exec();
        return;
    }
    QString strIp,strName,strCode;
    mutex1.lock();
    while(query.next())
    {
        strIp = query.value(2).toString();
        strName = query.value(7).toString();
        strCode = toolCode(strName);
        if(g_ip_map.contains(strIp))
        {
            for(int i=0;i<g_ip_map[strIp].size();i++)
            {
                if(g_ip_map[strIp][i]==strCode)
                {
                    break;
                }
                g_ip_map[strIp].append(strCode);
            }
        }
        else
        {
            QStringList strList;
            strList.append(strCode);
            g_ip_map.insert(strIp,strList);
        }
    }
    mutex1.unlock();
    attackTableWidgetShow();
}

void disposeDialog::logTableShow()
{
    ui->logTableWidget->verticalHeader()->setVisible(false);
    for(int i=0;i<3;i++)
    {
        ui->logTableWidget->setColumnWidth(i,120);
    }
    ui->logTableWidget->setColumnWidth(3,406);
}

void disposeDialog::finisgedSlot(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  // bytes
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(bytes, &jsonError);  // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {
            if(doucment.isArray())
            {
                QString strIp,strTime,strDis,strName;
                QSqlQuery query;
                //更新数据库中的内容
                QString strSql = QString("DELETE FROM logreport WHERE task_id=%1 AND ip='%2'").arg(g_task_id).arg(logIp);
                bool ok = query.exec(strSql);
                if(ok)
                {
                }
                else
                {
                    qWarning()<<"sql err:"<<query.lastError();
                }
                QJsonArray array = doucment.array();
                int arraySize = array.size();
                ui->logTableWidget->setRowCount(arraySize);
                for (int i = 0; i < arraySize; ++i)
                {
                    ui->logTableWidget->setRowHeight(i,60);
                    QJsonValue value = array.at(i);
                    if(value.isObject())
                    {
                        QJsonObject objValue = value.toObject();
                        if (objValue.contains("alertDate"))
                        {
                             QJsonValue alertValue = objValue.value("alertDate");
                             if(alertValue.isDouble())
                             {
                                 qlonglong nTime = alertValue.toVariant().toLongLong();
                                 QDateTime time = QDateTime::fromMSecsSinceEpoch(nTime);
                                 strTime = time.toString("yyyy-MM-dd hh:mm ddd");
                                 QTableWidgetItem *item1 = new QTableWidgetItem(strTime);
                                 ui->logTableWidget->setItem(i,1,item1);
                             }
                        }
                        if(objValue.contains("displayDetail"))
                        {
                            QJsonValue displayValue = objValue.value("displayDetail");
                            if(displayValue.isString())
                            {
                                strDis = displayValue.toVariant().toString();
                                QTableWidgetItem *item2 = new QTableWidgetItem(strDis);
                                ui->logTableWidget->setItem(i,3,item2);
                            }
                        }
                        if(objValue.contains("objectAddress"))
                        {
                            QJsonValue ipValue = objValue.value("objectAddress");
                            if(ipValue.isString())
                            {
                                strIp = ipValue.toVariant().toString();
                                QTableWidgetItem *item3 = new QTableWidgetItem(strIp);
                                ui->logTableWidget->setItem(i,2,item3);
                            }
                        }
                        if(objValue.contains("ruleName"))
                        {
                            QJsonValue nameValue = objValue.value("ruleName");
                            if(nameValue.isString())
                            {
                                strName = nameValue.toVariant().toString();
                                QTableWidgetItem *item4 = new QTableWidgetItem(strName);
                                ui->logTableWidget->setItem(i,0,item4);
                            }
                        }
                    }
                    strSql = QString("INSERT INTO logreport SET task_id=%1,code='%2',"
                                     "time='%3',ip='%4',detail='%5'").arg(g_task_id).arg(strName)
                            .arg(strTime).arg(strIp).arg(strDis);
                    bool ok2 = query.exec(strSql);
                    if(!ok2)
                    {
                        qWarning()<<"sql err:"<<query.lastError();
                    }
                }
                connect(ui->logTableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),
                        ui->logTableWidget,SLOT(resizeRowsToContents()) );
            }
            ui->logStackedWidget->setCurrentIndex(1);
        }
        else
        {
            ui->logStackedWidget->setCurrentIndex(0);
            QString strTitle = QString("提示");
            QString strMsg = QString("该资产无告警信息");
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
    else
    {
        ui->logStackedWidget->setCurrentIndex(0);
        QString strErr = QString("error:%1 code:%2").arg(reply->errorString()).arg(reply->error());
        qWarning()<<"request log result api error:"<<strErr;
        QString strTitle = QString("警告");
        QString strMsg = QString("无法连接日志分析平台");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    reply->deleteLater();
}

void disposeDialog::on_checkLogButton_clicked()
{
    int row = ui->anaTableWidget->currentRow();
    if(row==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    logIp = ui->anaTableWidget->item(row,1)->text();
    if(logIp.isEmpty())
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("资产IP为空");
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
        ui->logStackedWidget->setCurrentIndex(2);
        QMovie *pMovie = new QMovie(":/image/load.gif");
        ui->loadLabel->setScaledContents(true);
        ui->loadLabel->setMovie(pMovie);
        pMovie->start();
        send = new QNetworkAccessManager(this);
        QObject::connect(send,SIGNAL(finished(QNetworkReply*)),this,SLOT(finisgedSlot(QNetworkReply*)));

        QString strUrl = "https://172.16.39.88:8443/api/alertIP/"+logIp+"/100";
        QUrl url(strUrl);

        QNetworkRequest request;
        // 发送https请求前准备工作;
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        conf.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(conf);

        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json;charset=UTF-8"));
        request.setUrl(url);
        send->get(request);
    }
}

void disposeDialog::on_logReturnButton_clicked()
{
    ui->logStackedWidget->setCurrentIndex(0);
}


//将日志告警添加至页面
void disposeDialog::on_addReportButton_clicked()
{
    int row = ui->anaTableWidget->currentRow();
    if(row==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("请选择资产");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QString strIp = ui->anaTableWidget->item(row,1)->text();
    if(strIp.isEmpty())
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("资产IP为空");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QString strTitle = QString("添加");
    QString strMsg = QString("是否将该资产的日志告警添加至报告？");
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
        QString strSql = QString("SELECT task_id FROM logreport WHERE task_id=%1 AND ip='%2'")
                .arg(g_task_id).arg(logIp);
        QSqlQuery query;
        bool ok = query.exec(strSql);
        if(ok)
        {
            int count = query.size();
            if(count>0)
            {
                connect(this,SIGNAL(addLogToReport(QString)),mmw,SLOT(log_table_show_slot(QString)));
                emit addLogToReport(logIp);
                QString strTitle = QString("成功");
                QString strMsg = QString("已将该资产的日志告警添加至报告");
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
                QString strTitle = QString("错误");
                QString strMsg = QString("未获取到该资产的日志告警");
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
        else
        {
            qWarning()<<"sql err:"<<query.lastError();
        }
    }
}
