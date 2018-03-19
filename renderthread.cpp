#include "renderthread.h"
#include "commonfunction.h"
#include <QDir>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSettings>
#include <QDomDocument>

RenderThread::RenderThread(QObject *parent) : QThread(parent)
{
    stopped = false;
}

RenderThread::~RenderThread()
{
    qDebug() << "RenderThread::~RenderThread()";
}

void RenderThread::test()
{
    start(HighestPriority);
}

void RenderThread::stop()
{
    stopped = true;
}

QString toolName(QString toolCode)
{
    QString strName;
    if(toolCode=="001")
    {
        strName = QString("Windows主机配置检查工具");
    }
    else if(toolCode=="003")
    {
        strName = QString("病毒检查工具");
    }
    else if(toolCode=="004")
    {
        strName = QString("木马检查工具");
    }
    else if(toolCode=="006")
    {
        strName = QString("网站恶意代码检查工具");
    }
    else if(toolCode=="011")
    {
        strName = QString("系统漏洞检查工具");
    }
    else if(toolCode=="012")
    {
        strName = QString("网站安全检查工具");
    }
    else if(toolCode=="013")
    {
        strName = QString("数据库安全检查工具");
    }
    else if(toolCode=="002")
    {
        strName = QString("Linux主机配置检查工具");
    }
    else if(toolCode=="010")
    {
        strName = QString("SQL注入验证工具");
    }
    return strName;
}

void RenderThread::run()
{
    while(!stopped)
    {
        QString localPath = qApp->applicationDirPath();
        localPath+="\\config.ini";
        QSettings settings(localPath,QSettings::IniFormat);
        QString onlinePath = settings.value("autoscan/OnlineScanDir").toString();
        QString offlinePath = settings.value("autoscan/ScanDir").toString();

        QMap<QString,QStringList>::iterator it;
        for(it=g_ip_map.begin();it!=g_ip_map.end();++it)
        {
            QStringList mList = it.value();
            for(int i=0;i<mList.size();i++)
            {
                QString strId = QString("%1").arg(g_task_id);
                QString mIp = it.key();
                mIp.remove(".");
                QString strPath = onlinePath+"\\"+strId+"\\"+mIp+"\\"+mList[i];
                QString strSql;
                QSqlQuery query;
                //判断路径是否存在
                QDir dir(strPath);
                if(dir.exists())
                {
                    QStringList filters;
                    filters<<QString("*.xml");
                    dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
                    dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式
                    int dir_count = dir.count();
                    if(dir_count > 0)
                    {
                        QString strName = toolName(mList[i]);
                        strSql = QString("UPDATE check_task SET operate_state=2 WHERE ip_address='%1'"
                                                                "AND run_tool='%2'").arg(it.key()).arg(strName);
                        query.exec(strSql);
                        break;
                    }
                }

                QStringList file_list;
                //获取分隔符
                QChar separator = QChar('/');
                if(!offlinePath.contains(separator))
                {
                    separator = QChar('\\');
                }
                QChar last_char = offlinePath.at(offlinePath.length()-1);
                if(last_char == separator)
                {
                    separator = QChar();
                }
                QDir dir_1(offlinePath);
                if(!dir_1.exists())
                {
                    break;
                }
                QStringList filters_1;
                filters_1<<QString("*.xml");
                dir_1.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
                dir_1.setNameFilters(filters_1);

                int dir_count_2 = dir_1.count();
                for(int z=0;z<dir_count_2;z++)
                {
                    QString file_name = dir_1[z];  //文件名称
                    QString file_path = offlinePath + separator + file_name;   //文件全路径
                    file_list.append(file_path);
                }

                //系统漏扫结果目录
                if(!onlinePath.contains(separator))
                {
                    separator = QChar('\\');
                }
                QChar last_char1 = onlinePath.at(onlinePath.length()-1);
                if(last_char1 == separator)
                {
                    separator = QChar();
                }
                QDir dir_2(onlinePath);
                if(!dir_2.exists())
                {
                    break;
                }
                QStringList filters_2;
                filters_2<<QString("*.xml");
                dir_2.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
                dir_2.setNameFilters(filters_1);

                int dir_count_3 = dir_2.count();
                for(int l=0;l<dir_count_3;l++)
                {
                    QString file_name = dir_2[l];  //文件名称
                    QString file_path = onlinePath + separator + file_name;   //文件全路径
                    file_list.append(file_path);
                }


                QDomDocument doc;
                for(int j=0;j<file_list.size();j++)
                {
                    QString xmlPath = file_list[j];
                    QFile file3(xmlPath);
                    if (!file3.open(QFile::ReadOnly | QFile::Text))
                    {
                        qWarning()<<"open file error: "<<xmlPath.toStdString().c_str();
                        continue;
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

                    if(toolCode==mList[i])
                    {
                        QStringList ip_list = destIp.split(";");
                        for(int k=0;k<ip_list.size();k++)
                        {
                            if(ip_list[k]==it.key())
                            {
                                QString strName = toolName(mList[i]);
                                strSql = QString("UPDATE check_task SET operate_state=2 WHERE ip_address='%1'"
                                                                        "AND run_tool='%2' AND operate_state<>2")
                                        .arg(it.key()).arg(strName);
                                query.exec(strSql);                  
                                break;
                            }
                        }
                    }
                }
            }
        }
        sleep(5);
    }
}
