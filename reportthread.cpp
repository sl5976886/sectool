#include "reportthread.h"
#include "commonfunction.h"
#include <QDir>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDomDocument>
#include <QSettings>
#include <QLibrary>
#include <QDirIterator>
#include <QByteArray>
#include <QDateTime>

ReportThread::ReportThread(QObject *parent) : QThread(parent)
{
    stopped = false;
}


void ReportThread::test()
{
    start(HighestPriority);
}

void ReportThread::stop()
{
    stopped = true;
}

void ReportThread::getInfo(QString file, QString time)
{
    saveFileName = file;
    strTime = time;
}

bool ifHaveIp2(QString ip,QStringList ip_list)
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

void analyzeXml2(QString code,QStringList &list)
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

         //获取所选文件类型过滤器
        QStringList filters;
        filters<<QString("*.xml");

         //定义迭代器并设置过滤器
        QDir dir(mOnlinePath);
        QStringList online_list;
        if(dir.exists())
        {
            QDirIterator dir_iterator(mOnlinePath,
                filters,
                QDir::Files | QDir::NoSymLinks,
                QDirIterator::Subdirectories);
            while(dir_iterator.hasNext())
            {
                dir_iterator.next();
                QFileInfo file_info =dir_iterator.fileInfo();
                QString absolute_file_path =file_info.absoluteFilePath();
                online_list.append(absolute_file_path);
            }
        }

        //已经解析完的xml也要去匹配
        QString mBakPath = settings.value("autoscan/comfilepath").toString();
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

                QDomNodeList nodeswitch=docElem.elementsByTagName("task_info");
                QDomNode node = nodeswitch.item(0);
                QDomElement elemnodeswitch=node.toElement();
                QString taskCode = qPrintable(elemnodeswitch.attribute("taskcode"));
                QString strTaskId = QString::number(g_task_id,10);
                if(taskCode!=strTaskId)
                {
                    continue;
                }

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
                if(list.contains(strIp))
                {
                    continue;
                }
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

                QDomNodeList nodeswitch=docElem.elementsByTagName("task_info");
                QDomNode node = nodeswitch.item(0);
                QDomElement elemnodeswitch=node.toElement();
                QString taskCode = qPrintable(elemnodeswitch.attribute("taskcode"));
                QString strTaskId = QString::number(g_task_id,10);
                if(taskCode!=strTaskId)
                {
                    continue;
                }

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

                QDomNodeList nodeswitch=docElem.elementsByTagName("task_info");
                QDomNode node = nodeswitch.item(0);
                QDomElement elemnodeswitch=node.toElement();
                QString taskCode = qPrintable(elemnodeswitch.attribute("taskcode"));
                QString strTaskId = QString::number(g_task_id,10);
                if(taskCode!=strTaskId)
                {
                    continue;
                }

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

        //注：以前没有检验已经解析完的xml，所以这边需要判断，现在不需要
        int dir_count = dir.count();
//        if(dir_count <= 0)
//        {
//            return;
//        }

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

        if(dir_count>0)
        {
            for(int i=0;i<dir_count;i++)
            {
                QString file_name = dir[i];  //文件名称
                QString file_path = mOfflinePath + separator + file_name;   //文件全路径
                file_list.append(file_path);
            }
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
                strSql = QString("SELECT ip_address FROM check_task WHERE id=%1 AND run_tool='网站恶意代码检查工具' AND ifWrite=0")
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

                QDomNodeList nodeswitch_5=docElem.elementsByTagName("task_info");
                QDomNode node_5 = nodeswitch_5.item(0);
                QDomElement elemnodeswitch_5=node_5.toElement();
                QString taskCode = elemnodeswitch_5.attribute("taskcode");
                QString strTaskId = QString::number(g_task_id,10);
                if(taskCode!=strTaskId)
                {
                    continue;
                }

                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp_2 = element_2.attribute("src_ip");
                QStringList str_list = strIp_2.split(";");
                int count = str_list.count();
                QString hostIp = str_list[count-2];
                if(ifHaveIp2(hostIp,ip_list))
                {
                    list.append(hostIp);
                }

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

                QDomNodeList nodeswitch_5=docElem.elementsByTagName("task_info");
                QDomNode node_5 = nodeswitch_5.item(0);
                QDomElement elemnodeswitch_5=node_5.toElement();
                QString taskCode = elemnodeswitch_5.attribute("taskcode");
                QString strTaskId = QString::number(g_task_id,10);
                if(taskCode!=strTaskId)
                {
                    continue;
                }

                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp_2 = element_2.attribute("src_ip");
                QStringList str_list = strIp_2.split(";");
                int count = str_list.count();
                QString hostIp = str_list[count-2];
                if(ifHaveIp2(hostIp,ip_list))
                {
                    list.append(hostIp);
                }

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

                QDomNodeList nodeswitch_5=docElem.elementsByTagName("task_info");
                QDomNode node_5 = nodeswitch_5.item(0);
                QDomElement elemnodeswitch_5=node_5.toElement();
                QString taskCode = elemnodeswitch_5.attribute("taskcode");
                QString strTaskId = QString::number(g_task_id,10);
                if(taskCode!=strTaskId)
                {
                    continue;
                }

                QDomNodeList nodeswitch_2 = docElem.elementsByTagName("operation_info");
                QDomNode node_2 = nodeswitch_2.item(0);
                QDomElement element_2 = node_2.toElement();
                QString strIp_2 = element_2.attribute("src_ip");
                QStringList str_list = strIp_2.split(";");
                int count = str_list.count();
                QString hostIp = str_list[count-2];
                if(ifHaveIp2(hostIp,ip_list))
                {
                    list.append(hostIp);
                }

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

typedef void (*DLLSetCellText)(int,int,char*);
typedef void (*DLLGetTable)(int);
typedef void (*DLLInsertRow)();

void FillTable1(int index,QStringList vc,int col_size,DLLGetTable dllGetTable,DLLInsertRow dllInsertRow,DLLSetCellText dllSetCellText)
{
    dllGetTable(index);//注意表序号
    int mRow = vc.size()/col_size;
    for(int i = 0; i < mRow; i++)
    {
        dllInsertRow();
        for(int j=0;j<col_size;j++)
        {
            char *strText;
            QString mStr = vc[i*col_size+j];
            QByteArray ba = mStr.toLocal8Bit();
            strText=ba.data();
            dllSetCellText(i+1, j, strText);
        }
    }
}

void ReportThread::run()
{
    if(!stopped)
    {
        QLibrary mylib("WordEngine.dll");
        typedef bool (*DLLLoadFile)(char*,char*);
        typedef void (*DLLReplaceText)(char*,char*);
        if(mylib.load())
        {
            DLLLoadFile dllLoadFile = (DLLLoadFile)mylib.resolve("LoadFile");
            if(!dllLoadFile)
            {
                qWarning()<<"调用WordEngine出错:LoadFile error";
                emit ifSuccess(3);
                return;
            }

            DLLReplaceText dllReplaceText = (DLLReplaceText)mylib.resolve("ReplaceText");
            if(!dllReplaceText)
            {
                qWarning()<<"调用WordEngine出错:ReplaceText error";
                emit ifSuccess(3);
                return;
            }

            DLLGetTable dllGetTable = (DLLGetTable)mylib.resolve("GetTable");
            if(!dllGetTable)
            {
                qWarning()<<"调用WordEngine出错:GetTable error";
                emit ifSuccess(3);
                return;
            }

            DLLInsertRow dllInsertRow = (DLLInsertRow)mylib.resolve("InsertRow");
            if(!dllInsertRow)
            {
                qWarning()<<"调用WordEngine出错:InsertRow error";
                emit ifSuccess(3);
                return;
            }

            DLLSetCellText dllSetCellText = (DLLSetCellText)mylib.resolve("SetCellText");
            if(!dllSetCellText)
            {
                qWarning()<<"调用WordEngine出错:SetCellText error";
                emit ifSuccess(3);
                return;
            }

            typedef void (*DLLSaveFile)();
            DLLSaveFile dllSaveFile = (DLLSaveFile)mylib.resolve("SaveFile");
            if(!dllSaveFile)
            {
                qWarning()<<"调用WordEngine出错:SaveFile error";
                emit ifSuccess(3);
                return;
            }

            typedef void (*DLLUpdateDirectory)(char*);
            DLLUpdateDirectory dllUpdateDirectory = (DLLUpdateDirectory)mylib.resolve("UpdateDirectory");
            if(!dllUpdateDirectory)
            {
                qWarning()<<"调用WordEngine出错:UpdateDirectory error";
                emit ifSuccess(3);
                return;
            }

            typedef void (*DLLSetTableProperty)(int,int);
            DLLSetTableProperty dllSetTableProperty = (DLLSetTableProperty)mylib.resolve("SetTableProperty");
            if(!dllSetTableProperty)
            {
                qWarning()<<"调用WordEngine出错:SetTableProperty error";
                emit ifSuccess(3);
                return;
            }

            //生成报告
            QString tpFilePath = qApp->applicationDirPath();
            tpFilePath += QString("\\模板文件\\");
            tpFilePath += QString("应急处置工具箱__报告(模板).docx");
            char *strTpFile;
            QByteArray ba_1 = tpFilePath.toLocal8Bit();
            strTpFile = ba_1.data();

            char *strSaveFile;
            QByteArray ba_2 = saveFileName.toLocal8Bit();
            strSaveFile = ba_2.data();
            if(!dllLoadFile(strTpFile,strSaveFile))
            {
                qWarning()<<"加载word模板失败： "<<tpFilePath.toStdString().c_str();
                emit ifSuccess(2);
                return;
            }

            //封面
            QSqlQuery query;
            QString strSql;
            strSql = QString("SELECT * FROM main_task WHERE id=%1").arg(g_task_id);
            bool ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
                emit ifSuccess(1);
                return;
            }
            QString str;
            QByteArray mBa;
            char *cStr;
            if(query.next())
            {
                str = query.value(2).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{00001}",cStr);

                str = query.value(8).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{00002}",cStr);

                str = query.value(9).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{00003}",cStr);

                str = query.value(4).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{00004}",cStr);

                str = query.value(16).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{00005}",cStr);

                mBa = g_user_name.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{00006}",cStr);

                mBa = strTime.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{00007}",cStr);
            }

            //网站描述、网站开发架构基本情况
            strSql = QString("SELECT * FROM safe_record_form WHERE id=%1").arg(g_task_id);
            ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
                emit ifSuccess(1);
                return;
            }
            if(query.next())
            {
                QString mChName = query.value(1).toString();
                if(mChName.isEmpty())
                {
                    mChName = QString("未填写");
                }
                QString mSysName = query.value(3).toString();
                if(mSysName.isEmpty())
                {
                    mSysName = QString("未填写");
                }
                QString mIp = query.value(6).toString();
                if(mIp.isEmpty())
                {
                    mIp = QString("未填写");
                }
                QString mWebsite = query.value(5).toString();
                if(mWebsite.isEmpty())
                {
                    mWebsite = QString("未填写");
                }
                QString mDiscribe = QString("单位名称为：%1\n系统名称为：%2\n网站IP地址为：%3\n网站域名为：%4\n")
                        .arg(mChName).arg(mSysName).arg(mIp).arg(mWebsite);
                mBa = mDiscribe.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{11000}",cStr);

                //以下元素命名经过修改所以全都不匹配
                QString mLanguage;
                QDateTime mTime1 = query.value(14).toDateTime();
                if(mTime1.isValid())
                {
                    mLanguage = mTime1.toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    mLanguage = QString("未填写");
                }
                QString mBuild = query.value(15).toString();
                if(mBuild.isEmpty())
                {
                    mBuild = QString("未填写");
                }
                QDateTime mTime2 = query.value(17).toDateTime();
                QString mMiddle;
                if(mTime2.isValid())
                {
                    mMiddle = mTime2.toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    mMiddle = QString("未填写");
                }

                QString mData = query.value(18).toString();
                if(mData.isEmpty())
                {
                    mData = QString("未填写");
                }
                QString mServer = query.value(21).toString();
                if(mServer.isEmpty())
                {
                    mServer = QString("未填写");
                }
                QString mRemote = query.value(22).toString();
                if(mRemote.isEmpty())
                {
                    mRemote = QString("未填写");
                }
                QString mJiagou = QString("发现时间：%1\n发现来源：%2\n遭受攻击时间：%3\n系统遭破坏程度：%4\n调查单位：%5\n"
                                                         "调查人：%6\n").arg(mLanguage).arg(mBuild).arg(mMiddle)
                        .arg(mData).arg(mServer).arg(mRemote);
                mBa = mJiagou.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{12000}",cStr);
            }

            //网站资产情况
            strSql = QString("SELECT * FROM check_task WHERE id=%1").arg(g_task_id);
            ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
                emit ifSuccess(1);
                return;
            }
            QStringList hostList;
            QString mIp,mOperate,mName,mMiddle,mKind;
            QMap<QString,QString> mHostMap;
            while(query.next())
            {
                mIp = query.value(2).toString();
                mOperate = query.value(6).toString();
                mName = query.value(1).toString();
                mMiddle = query.value(5).toString();
                mKind = query.value(3).toString();
                if(mHostMap.contains(mIp))
                {
                    QString hostName = mHostMap[mIp];
                    if(hostName==mName)
                    {
                        continue;
                    }
                    else
                    {
                        mHostMap.insert(mIp,mName);
                    }
                }
                hostList.append(mIp);
                hostList.append(mName);
                hostList.append(mOperate);
                hostList.append(mMiddle);
                hostList.append(mKind);
            }
            FillTable1(1,hostList,5,dllGetTable,dllInsertRow,dllSetCellText);


            //网站安全漏洞
            QStringList scan_list;
            QString toolCode;
            //webscan
            toolCode = QString("012");
            analyzeXml2(toolCode,scan_list);
            FillTable1(2,scan_list,5,dllGetTable,dllInsertRow,dllSetCellText);
            scan_list.clear();
            //系统漏扫
            toolCode = QString("011");
            analyzeXml2(toolCode,scan_list);
            FillTable1(4,scan_list,5,dllGetTable,dllInsertRow,dllSetCellText);
            scan_list.clear();
            //数据库安全
            toolCode = QString("013");
            analyzeXml2(toolCode,scan_list);
            FillTable1(5,scan_list,5,dllGetTable,dllInsertRow,dllSetCellText);
            scan_list.clear();
            //恶意代码
            toolCode = QString("006");
            analyzeXml2(toolCode,scan_list);
            FillTable1(3,scan_list,4,dllGetTable,dllInsertRow,dllSetCellText);
            scan_list.clear();
            //木马
            toolCode = QString("004");
            analyzeXml2(toolCode,scan_list);
            FillTable1(6,scan_list,4,dllGetTable,dllInsertRow,dllSetCellText);
            scan_list.clear();
            //病毒
            toolCode = QString("003");
            analyzeXml2(toolCode,scan_list);
            FillTable1(7,scan_list,4,dllGetTable,dllInsertRow,dllSetCellText);
            scan_list.clear();

            //日志分析
            strSql = QString("SELECT * FROM logreport WHERE task_id=%1").arg(g_task_id);
            ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
                emit ifSuccess(1);
                return;
            }
            else
            {
                QStringList mList;
                mList.clear();
                while (query.next())
                {
                    QString strCode = query.value(1).toString();
                    mList.append(strCode);
                    QString mStrTime = query.value(2).toString();
                    mList.append(mStrTime);
                    QString strIp = query.value(3).toString();
                    mList.append(strIp);
                    QString strDis = query.value(4).toString();
                    mList.append(strDis);
                }
                FillTable1(8,mList,4,dllGetTable,dllInsertRow,dllSetCellText);
            }

            //应急处置情况
            strSql = QString("SELECT * FROM website_condition WHERE id=%1").arg(g_task_id);
            ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
                emit ifSuccess(1);
            }
            if(query.next())
            {
                str = query.value(2).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{41000}",cStr);

                str = query.value(3).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{42000}",cStr);

                str = query.value(4).toString();
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{43000}",cStr);
            }
            else
            {
                str = QString("未填写过程");
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{41000}",cStr);

                str = QString("未填写原因");
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{42000}",cStr);

                str = QString("未填写结果");
                mBa = str.toLocal8Bit();
                cStr = mBa.data();
                dllReplaceText("{43000}",cStr);
            }

            dllSaveFile();
            emit ifSuccess(0);
            return;
        }
    }
}
