#include "taskmanagedialog.h"
#include "ui_taskmanagedialog.h"
#include "commonfunction.h"
#include "disposedialog.h"
#include "modifydialog.h"
#include "newtaskdialog.h"
#include <QDebug>
#include <QPoint>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDomDocument>
#include <QMap>

taskManageDialog::taskManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::taskManageDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("应急处置任务"));
    this->setFixedSize(this->width(),this->height());
    QString action = QString("任务管理");
    QString state = QString("成功");
    operateLog(action,state);
    taskTableViewInit();
    ui->saveButton->setVisible(false);

    QString style = QString("QPushButton{background-color: rgb(102, 102, 102);border-style:none;border:0px;}");
    ui->modifyButton->setStyleSheet(style);
    ui->closeButton->setStyleSheet(style);
    ui->deleteButton->setStyleSheet(style);
    ui->newButton->setStyleSheet(style);
    ui->openButton->setStyleSheet(style);
    ui->saveButton->setStyleSheet(style);
}

taskManageDialog::~taskManageDialog()
{
    delete ui;
}

void taskManageDialog::task_fresh_slot()
{
    taskTableViewInit();
}

void taskManageDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QSqlQuery query_o;
        QString strSql_o = QString("SELECT * FROM main_task WHERE state='关闭'");
        query_o.exec(strSql_o);
        int queryCount = query_o.size();
        if(g_task_id!=0&&queryCount!=0)
        {
            QString strTitle = QString("已有打开的任务");
            QString strMsg = QString("你确定关闭之前的任务吗？");
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
                QSqlQuery query1;
                QString strSql1 = QString("UPDATE main_task SET state = '关闭' WHERE id = %1").arg(g_task_id);
                query1.exec(strSql1);
            }
        }
        int curRow = ui->taskTableView->currentIndex().row();
        if(curRow==-1)
        {
            QString strTitle = QString("警告");
            QString strMsg = QString("没有选中的任务");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
        QModelIndex index=taskTableModel->index(curRow,0,QModelIndex());
        g_task_id = index.data().toInt();
        QSqlQuery query;
        QString strSql = QString("UPDATE main_task SET state = '开启' WHERE id = '%1'").arg(g_task_id);
        bool ok = query.exec(strSql);
        if(!ok)
        {
            //error log
        }
        this->close();
        disposeDialog dpd(this);
        dpd.setModal(true);
        dpd.show();
        dpd.exec();
    }
}

//新建任务按钮
void taskManageDialog::on_newButton_clicked()
{
//    this->hide();
    newTaskDialog ntd(this);
    ntd.hookTaskManage(this);
    ntd.setModal(true);
    ntd.show();
    ntd.exec();
}

void taskManageDialog::double_click_slot(QModelIndex index)
{
    int curRow = index.row();
    QModelIndex index_2=taskTableModel->index(curRow,1,QModelIndex());
    QString strState = index_2.data().toString();
    QSqlQuery query_o;
    QString strSql_o = QString("SELECT * FROM main_task WHERE state='开启'");
    query_o.exec(strSql_o);
    int queryCount = query_o.size();
    if(g_task_id!=0&&queryCount>1)
    {
        QString strTitle = QString("已有打开的任务");
        QString strMsg = QString("你确定关闭之前的任务吗？");
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, strMsg, QMessageBox::Ok|QMessageBox::No,0);
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
            QSqlQuery query1;
            QString strSql1 = QString("UPDATE main_task SET state = '关闭' WHERE id = %1").arg(g_task_id);
            query1.exec(strSql1);
        }
    }
    else if(g_task_id!=0&&queryCount!=0&&strState==QString("关闭"))
    {
        QString strTitle = QString("已有打开的任务");
        QString strMsg = QString("你确定关闭之前的任务吗？");
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
            QSqlQuery query1;
            QString strSql1 = QString("UPDATE main_task SET state = '关闭' WHERE id = %1").arg(g_task_id);
            query1.exec(strSql1);
        }
    }
    if(curRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QModelIndex index_1=taskTableModel->index(curRow,0,QModelIndex());
    g_task_id = index_1.data().toInt();
    QSqlQuery query;
    QString strSql = QString("UPDATE main_task SET state = '开启' WHERE id = '%1'").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        //error log
    }
    this->close();

    connect(this,SIGNAL(openTask()),mmw,SLOT(openTaskShow_slot()));
    emit openTask();
}

//TableView init
void taskManageDialog::taskTableViewInit()
{
    taskTableModel = new QSqlTableModel(this);
    taskTableModel->setTable("main_task");
    taskTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    taskTableModel->setFilter(QObject::tr("user_name='%1'").arg(g_user_name));
    taskTableModel->select();
    taskTableModel->setHeaderData(0,Qt::Horizontal,QString("任务ID"));
    taskTableModel->setHeaderData(1,Qt::Horizontal,QString("任务状态"));
    taskTableModel->setHeaderData(2,Qt::Horizontal,QString("事发单位"));
    taskTableModel->setHeaderData(3,Qt::Horizontal,QString("单位地址"));
    taskTableModel->setHeaderData(4,Qt::Horizontal,QString("负责人"));
    taskTableModel->setHeaderData(5,Qt::Horizontal,QString("联系电话"));
    taskTableModel->setHeaderData(6,Qt::Horizontal,QString("办公电话"));
    taskTableModel->setHeaderData(7,Qt::Horizontal,QString("电子邮箱"));
    taskTableModel->setHeaderData(8,Qt::Horizontal,QString("系统名称"));
    taskTableModel->setHeaderData(9,Qt::Horizontal,QString("系统等级"));
    taskTableModel->setHeaderData(10,Qt::Horizontal,QString("事件来源"));
    taskTableModel->setHeaderData(11,Qt::Horizontal,QString("事件名称"));
    taskTableModel->setHeaderData(12,Qt::Horizontal,QString("事发时间"));
    taskTableModel->setHeaderData(13,Qt::Horizontal,QString("事发地点"));
    taskTableModel->setHeaderData(14,Qt::Horizontal,QString("事件类型"));
    taskTableModel->setHeaderData(15,Qt::Horizontal,QString("事件级别"));
    taskTableModel->setHeaderData(16,Qt::Horizontal,QString("处置单位"));
    taskTableModel->setHeaderData(17,Qt::Horizontal,QString("事件描述"));
    ui->taskTableView->setModel(taskTableModel);
    ui->taskTableView->verticalHeader()->setVisible(false);
    ui->taskTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->taskTableView->horizontalHeader()->setSectionsClickable(false);
    //ui->taskTableView->hideColumn(0);
    ui->taskTableView->setColumnWidth(17,220);
    ui->taskTableView->setColumnWidth(15,90);
    ui->taskTableView->setColumnWidth(14,60);
    ui->taskTableView->setColumnWidth(12,110);
    ui->taskTableView->setColumnWidth(9,60);
    ui->taskTableView->setColumnWidth(10,60);
    ui->taskTableView->setColumnWidth(1,60);
    ui->taskTableView->setColumnWidth(0,60);
    ui->taskTableView->hideColumn(18);
    ui->taskTableView->hideColumn(19);
    for(int i=0;i<taskTableModel->rowCount();i++)
    {
        ui->taskTableView->setRowHeight(i,30);
    }
    ui->taskTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->taskTableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(double_click_slot(QModelIndex)));
}

//刷新显示
void taskManageDialog::refreshTableView()
{
    taskTableModel->select();
    taskTableModel->setHeaderData(0,Qt::Horizontal,QString("任务ID"));
    taskTableModel->setHeaderData(1,Qt::Horizontal,QString("任务状态"));
    taskTableModel->setHeaderData(2,Qt::Horizontal,QString("事发单位"));
    taskTableModel->setHeaderData(3,Qt::Horizontal,QString("单位地址"));
    taskTableModel->setHeaderData(4,Qt::Horizontal,QString("负责人"));
    taskTableModel->setHeaderData(5,Qt::Horizontal,QString("联系电话"));
    taskTableModel->setHeaderData(6,Qt::Horizontal,QString("办公电话"));
    taskTableModel->setHeaderData(7,Qt::Horizontal,QString("电子邮箱"));
    taskTableModel->setHeaderData(8,Qt::Horizontal,QString("系统名称"));
    taskTableModel->setHeaderData(9,Qt::Horizontal,QString("系统等级"));
    taskTableModel->setHeaderData(10,Qt::Horizontal,QString("事件来源"));
    taskTableModel->setHeaderData(11,Qt::Horizontal,QString("事件名称"));
    taskTableModel->setHeaderData(12,Qt::Horizontal,QString("事发时间"));
    taskTableModel->setHeaderData(13,Qt::Horizontal,QString("事发地点"));
    taskTableModel->setHeaderData(14,Qt::Horizontal,QString("事件类型"));
    taskTableModel->setHeaderData(15,Qt::Horizontal,QString("事件级别"));
    taskTableModel->setHeaderData(16,Qt::Horizontal,QString("处置单位"));
    taskTableModel->setHeaderData(17,Qt::Horizontal,QString("事件描述"));
    taskTableModel->setHeaderData(18,Qt::Horizontal,QString("保存位置"));
    ui->taskTableView->setModel(taskTableModel);
    ui->taskTableView->verticalHeader()->setVisible(false);
    ui->taskTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->taskTableView->hideColumn(0);
    ui->taskTableView->setColumnWidth(18,220);
    ui->taskTableView->setColumnWidth(17,220);
    ui->taskTableView->setColumnWidth(15,90);
    ui->taskTableView->setColumnWidth(14,60);
    ui->taskTableView->setColumnWidth(12,110);
    ui->taskTableView->setColumnWidth(9,60);
    ui->taskTableView->setColumnWidth(10,60);
    ui->taskTableView->setColumnWidth(1,60);
    ui->taskTableView->setColumnWidth(0,60);
    for(int i=0;i<taskTableModel->rowCount();i++)
    {
        ui->taskTableView->setRowHeight(i,30);
    }
}

void taskManageDialog::other_refresh()
{
    refreshTableView();
}

//打开任务
void taskManageDialog::on_openButton_clicked()
{
    int curRow = ui->taskTableView->currentIndex().row();
    QModelIndex index_1=taskTableModel->index(curRow,1,QModelIndex());
    QString strState = index_1.data().toString();
    QSqlQuery query_o;
    QString strSql_o = QString("SELECT * FROM main_task WHERE state='开启'");
    query_o.exec(strSql_o);
    int queryCount = query_o.size();
    if(g_task_id!=0&&queryCount>1)
    {
        QString strTitle = QString("已有打开的任务");
        QString strMsg = QString("你确定关闭之前的任务吗？");
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, strMsg, QMessageBox::Ok|QMessageBox::No,0);
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
            QSqlQuery query1;
            QString strSql1 = QString("UPDATE main_task SET state = '关闭' WHERE id = %1").arg(g_task_id);
            query1.exec(strSql1);
        }
    }
    else if(g_task_id!=0&&queryCount!=0&&strState==QString("关闭"))
    {
        QString strTitle = QString("已有打开的任务");
        QString strMsg = QString("你确定关闭之前的任务吗？");
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
            QSqlQuery query1;
            QString strSql1 = QString("UPDATE main_task SET state = '关闭' WHERE id = %1").arg(g_task_id);
            query1.exec(strSql1);
        }
    }
    if(curRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QModelIndex index=taskTableModel->index(curRow,0,QModelIndex());
    g_task_id = index.data().toInt();
    QSqlQuery query;
    QString strSql = QString("UPDATE main_task SET state = '开启' WHERE id = '%1'").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        //error log
    }
    this->close();
    //原来如下
//    disposeDialog dpd;
//    dpd.setModal(true);
//    dpd.show();
//    dpd.exec();

    connect(this,SIGNAL(openTask()),mmw,SLOT(openTaskShow_slot()));
    emit openTask();
}

void taskManageDialog::hookMainDialog(mainDialog *w)
{
    mmw = w;
}

//删除任务
void taskManageDialog::on_deleteButton_clicked()
{
    //获取选中的行
    int curRow = ui->taskTableView->currentIndex().row();
    if(curRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QModelIndex index=taskTableModel->index(curRow,0,QModelIndex());
    int mTaskId = 0;
    mTaskId = index.data().toInt();
    QSqlQuery query;
    QString strSql = QString("SELECT state FROM main_task WHERE id = %1").arg(mTaskId);
    query.exec(strSql);
    int count = query.size();
    if(count==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有可供删除的任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }

    //删除选中的行
    QItemSelectionModel *selections = ui->taskTableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    if(selected.count()==0)
    {
        QString strTitle = QString("提示");
        QString strMsg = QString("请选中一个任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QMap<int, int> rowMap;
    foreach (QModelIndex index, selected)
    {
        rowMap.insert(index.row(),0);
    }
    int rowToDel;
    QMapIterator<int, int> rowMapIterator(rowMap);
    rowMapIterator.toBack();
    while (rowMapIterator.hasPrevious())
    {
        rowMapIterator.previous();
        rowToDel = rowMapIterator.key();
        taskTableModel->removeRow(rowToDel);
    }
    //删除该行
    //taskTableModel->removeRow(curRow);

    QString strTitle = QString("删除");
    QString strMsg = QString("你确定删除当前的任务吗？");
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
        taskTableModel->revertAll(); //如果不删除，则撤销
    }
    else
    {
        taskTableModel->submitAll(); //否则提交，在数据库中删除该行
        strSql = QString("DELETE FROM host_info WHERE id=%1").arg(mTaskId);
        bool mOk = query.exec(strSql);
        if(!mOk)
        {
            qWarning()<<"sql err:"<<query.lastError();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错，请重试");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
        strSql = QString("DELETE FROM check_task WHERE id=%1").arg(mTaskId);
        mOk = query.exec(strSql);
        if(!mOk)
        {
            qWarning()<<"sql err:"<<query.lastError();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错，请重试");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
        strSql = QString("DELETE FROM safe_record_form WHERE id=%1").arg(mTaskId);
        mOk = query.exec(strSql);
        if(!mOk)
        {
            qWarning()<<"sql err:"<<query.lastError();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错，请重试");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
        strSql = QString("DELETE FROM web_record_form WHERE id=%1").arg(mTaskId);
        mOk = query.exec(strSql);
        if(!mOk)
        {
            qWarning()<<"sql err:"<<query.lastError();
            QString strTitle = QString("错误");
            QString strMsg = QString("数据库出错，请重试");
            QString showMsg = "<font color='black'>"+strMsg+"</font>";
            QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
            if(NULL!=WrrMsg->button(QMessageBox::Ok))
            {
                WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
            }
            WrrMsg->exec();
            return;
        }
        qDebug()<<"delete task:"<<mTaskId;
        if(g_task_id==mTaskId)
        {
            g_task_id = 0;
        }
    }
}

//关闭任务
void taskManageDialog::on_closeButton_clicked()
{
    int curRow = ui->taskTableView->currentIndex().row();
    if(curRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }

    QModelIndex index=taskTableModel->index(curRow,1,QModelIndex());
    QString strState = index.data().toString();
    if(strState==QString("关闭"))
    {
        QString strTitle = QString("提示");
        QString strMsg = QString("当前任务已关闭");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }

    //关闭选中的任务
    QString strTitle = QString("关闭任务");
    QString strMsg = QString("你确定关闭选中的任务吗？");
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
        QItemSelectionModel *selections = ui->taskTableView->selectionModel();
        QModelIndexList selected = selections->selectedIndexes();
        QMap<int, int> rowMap;
        foreach (QModelIndex index, selected)
        {
            rowMap.insert(index.row(),0);
        }
        int rowToDel;
        QMapIterator<int, int> rowMapIterator(rowMap);
        rowMapIterator.toBack();
        while (rowMapIterator.hasPrevious())
        {
            rowMapIterator.previous();
            rowToDel = rowMapIterator.key();
            QModelIndex index=taskTableModel->index(rowToDel,0,QModelIndex());
            int mTaskId = 0;
            mTaskId = index.data().toInt();

            QSqlQuery query;
            QString strSql = QString("SELECT state FROM main_task WHERE id = %1").arg(mTaskId);
            query.exec(strSql);
            if(query.next())
            {
                QString state = query.value(0).toString();
                if(!(state == QString("关闭")))
                {
                    strSql = QString("UPDATE main_task SET state = '关闭' WHERE id = %1").arg(mTaskId);
                    query.exec(strSql);
                    qDebug()<<"close task:"<<g_task_id;
                    g_task_id = 0;
                    refreshTableView();
                }
            }
        }
    }
}

//刷新任务
//void taskManageDialog::on_closeButton_2_clicked()
//{
//    refreshTableView();
//}

//任务另存为
void taskManageDialog::on_saveButton_clicked()
{
    int curRow = ui->taskTableView->currentIndex().row();
    if(curRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QModelIndex index=taskTableModel->index(curRow,0,QModelIndex());
    int mTaskId = 0;
    mTaskId = index.data().toInt();;
    QString saveFileName = QFileDialog::getSaveFileName(this,QString("另存为任务"),".","(*.pro)");
    if(saveFileName.isEmpty())
    {
//        QString strTitle = QString("警告");
//        QString strMsg = QString("请选择文件");
//        QString showMsg = "<font color='black'>"+strMsg+"</font>";
//        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
//        if(NULL!=WrrMsg->button(QMessageBox::Ok))
//        {
//            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
//        }
//        WrrMsg->exec();
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

//修改任务
void taskManageDialog::on_modifyButton_clicked()
{
    int curRow = ui->taskTableView->currentIndex().row();
    if(curRow==-1)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("没有选中的任务");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    QModelIndex index=taskTableModel->index(curRow,0,QModelIndex());
    int mId = index.data().toInt();

    modifyDialog mdd(this);
    mdd.getTaskId(mId);
    connect(mdd.certainButton,SIGNAL(clicked()),this,SLOT(other_refresh()));
    mdd.setModal(true);
    mdd.show();
    mdd.exec();
}
