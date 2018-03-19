#include "starttool.h"
#include "commonfunction.h"
#include <QSettings>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QMessageBox>

startTool::startTool(QObject *parent) : QThread(parent)
{
    qInstallMessageHandler(outputMessage);
    stopped=false;
}

void startTool::test()
{
    start(HighestPriority);
}

void startTool::run()
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
    QString tool_4 = settings.value("onlinecheck/Linxu.ExecuteFileName").toString();
    while(!stopped)
    {
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
            bool ok = QProcess::startDetached(tool_1,QStringList());
            if(!ok)
            {
                qWarning()<<"start tool failed!";
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
            bool ok = QProcess::startDetached(tool_2,QStringList());
            if(!ok)
            {
                qWarning()<<"start tool failed!";
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
            bool ok = QProcess::startDetached(tool_3,QStringList());
            if(!ok)
            {
                qWarning()<<"start tool failed!";
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
            bool ok = QProcess::startDetached(tool_4,QStringList());
            if(!ok)
            {
                qWarning()<<"start tool failed!";
            }
        }
        sleep(30);
    }
}

void startTool::stop()
{
    stopped=true;
}
