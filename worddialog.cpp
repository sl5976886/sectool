#include "worddialog.h"
#include "ui_worddialog.h"
#include "commonfunction.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QLibrary>
#include <QDirIterator>
#include <QSettings>
#include <QDomDocument>
#include <QTextStream>
#include <QTextCodec>

wordDialog::wordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wordDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(QString("报告"));
    qInstallMessageHandler(outputMessage);
    initLabel();
//    ui->wordButton->setStyleSheet("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;font:87 17pt Arial Rounded MT Bold ;border-radius:5px;border-style:outset;}"
//                                  "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                                  "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");
}

wordDialog::~wordDialog()
{
    delete ui;
}

void wordDialog::initLabel()
{
    QSqlQuery query;
    QString strSql;
    strSql = QString("SELECT event_company,event_time,dispose_company FROM main_task WHERE id=%1").arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
    QString str;
    if(query.next())
    {
        str = query.value(0).toString();
        ui->resLabel_1->setText(str);
        QDateTime mTime = query.value(1).toDateTime();
        str = mTime.toString("yyyy-MM-dd");
        ui->resLabel_2->setText(str);
        str = query.value(2).toString();
        ui->resLabel_3->setText(str);
    }
    ui->resLabel_4->setText(g_user_name);
    QDateTime curTime = QDateTime::currentDateTime();
    strTime = curTime.toString("yyyy-MM-dd");
    ui->resLabel_5->setText(strTime);

}

typedef void (*DLLSetCellText)(int,int,char*);
typedef void (*DLLGetTable)(int);
typedef void (*DLLInsertRow)();

void FillTable(int index,QStringList vc,int col_size,DLLGetTable dllGetTable,DLLInsertRow dllInsertRow,DLLSetCellText dllSetCellText)
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

bool ifHaveIp(QString ip,QStringList ip_list)
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

void analyzeXml(QString code,QStringList &list)
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

        QSettings settings_2(localPath,QSettings::IniFormat);
        QString mOfflinePath = settings_2.value("autoscan/ScanDir").toString();

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
                if(!ifHaveIp(hostIp,ip_list))
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
                if(!ifHaveIp(hostIp,ip_list))
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
                if(!ifHaveIp(hostIp,ip_list))
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

//导出报告
void wordDialog::on_wordButton_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,QString("保存文件"),QDir::currentPath(),"(*.docx)");
    if(saveFileName.isEmpty())
    {
//        QMessageBox::information(this,QString("出错"),QString("请选择文件"));
        return;
    }
    QLibrary mylib("WordEngine.dll");
    typedef bool (*DLLLoadFile)(char*,char*);
    typedef void (*DLLReplaceText)(char*,char*);
    if(mylib.load())
    {
        DLLLoadFile dllLoadFile = (DLLLoadFile)mylib.resolve("LoadFile");
        if(!dllLoadFile)
        {
            qWarning()<<"调用WordEngine出错:LoadFile error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
            return;
        }

        DLLReplaceText dllReplaceText = (DLLReplaceText)mylib.resolve("ReplaceText");
        if(!dllReplaceText)
        {
            qWarning()<<"调用WordEngine出错:ReplaceText error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
            return;
        }

        DLLGetTable dllGetTable = (DLLGetTable)mylib.resolve("GetTable");
        if(!dllGetTable)
        {
            qWarning()<<"调用WordEngine出错:GetTable error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
            return;
        }

        DLLInsertRow dllInsertRow = (DLLInsertRow)mylib.resolve("InsertRow");
        if(!dllInsertRow)
        {
            qWarning()<<"调用WordEngine出错:InsertRow error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
            return;
        }

        DLLSetCellText dllSetCellText = (DLLSetCellText)mylib.resolve("SetCellText");
        if(!dllSetCellText)
        {
            qWarning()<<"调用WordEngine出错:SetCellText error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
            return;
        }

        typedef void (*DLLSaveFile)();
        DLLSaveFile dllSaveFile = (DLLSaveFile)mylib.resolve("SaveFile");
        if(!dllSaveFile)
        {
            qWarning()<<"调用WordEngine出错:SaveFile error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
            return;
        }

        typedef void (*DLLUpdateDirectory)(char*);
        DLLUpdateDirectory dllUpdateDirectory = (DLLUpdateDirectory)mylib.resolve("UpdateDirectory");
        if(!dllUpdateDirectory)
        {
            qWarning()<<"调用WordEngine出错:UpdateDirectory error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
            return;
        }

        typedef void (*DLLSetTableProperty)(int,int);
        DLLSetTableProperty dllSetTableProperty = (DLLSetTableProperty)mylib.resolve("SetTableProperty");
        if(!dllSetTableProperty)
        {
            qWarning()<<"调用WordEngine出错:SetTableProperty error";
            QMessageBox::warning(this,QString("错误"),QString("调用WordEngine出错"),
                                 QMessageBox::Yes);
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
            QMessageBox::warning(this,QString("错误"),QString("加载word模板失败,文件可能丢失或者被占用"),
                                 QMessageBox::Yes);
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
        strSql = QString("SELECT * FROM web_record_form WHERE id=%1").arg(g_task_id);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
        }
        if(query.next())
        {
            QString mChName = query.value(1).toString();
            QString mIp = query.value(2).toString();
            QString mWebsite = query.value(3).toString();
            QString mDiscribe = QString("网站中文名为：%1\n网站ip地址为：%2\n网站网址为：%3\n")
                    .arg(mChName).arg(mIp).arg(mWebsite);
            mBa = mDiscribe.toLocal8Bit();
            cStr = mBa.data();
            dllReplaceText("{11000}",cStr);

            QString mLanguage = query.value(20).toString();
            QString mBuild = query.value(21).toString();
            QString mMiddle = query.value(22).toString();
            QString mData = query.value(23).toString();
            QString mServer = query.value(24).toString();
            QString mRemote = query.value(25).toString();
            QString mJiagou = QString("开发语言：%1\n建站平台：%2\n中间件：%3\n数据库：%4\n服务器：%5\n"
                                                     "远程维护方式：%6\n").arg(mLanguage).arg(mBuild).arg(mMiddle)
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
        }
        QStringList hostList;
        QString mIp,mOperate,mDatabase,mMiddle,mKind;
        while(query.next())
        {
            mIp = query.value(2).toString();
            mOperate = query.value(6).toString();
            mDatabase = query.value(4).toString();
            mMiddle = query.value(5).toString();
            mKind = query.value(3).toString();
            hostList.append(mIp);
            hostList.append(mOperate);
            hostList.append(mDatabase);
            hostList.append(mMiddle);
            hostList.append(mKind);
        }
        FillTable(1,hostList,5,dllGetTable,dllInsertRow,dllSetCellText);

        //日志分析（暂时没有数据）

        //网站安全漏洞
        QStringList scan_list;
        QString toolCode;
        //webscan
        toolCode = QString("012");
        analyzeXml(toolCode,scan_list);
        FillTable(2,scan_list,5,dllGetTable,dllInsertRow,dllSetCellText);
        scan_list.clear();
        //系统漏扫
        toolCode = QString("011");
        analyzeXml(toolCode,scan_list);
        FillTable(4,scan_list,5,dllGetTable,dllInsertRow,dllSetCellText);
        scan_list.clear();
        //数据库安全
        toolCode = QString("013");
        analyzeXml(toolCode,scan_list);
        FillTable(5,scan_list,5,dllGetTable,dllInsertRow,dllSetCellText);
        scan_list.clear();
        //恶意代码
        toolCode = QString("006");
        analyzeXml(toolCode,scan_list);
        FillTable(3,scan_list,4,dllGetTable,dllInsertRow,dllSetCellText);
        scan_list.clear();
        //木马
        toolCode = QString("004");
        analyzeXml(toolCode,scan_list);
        FillTable(6,scan_list,4,dllGetTable,dllInsertRow,dllSetCellText);
        scan_list.clear();
        //病毒
        toolCode = QString("003");
        analyzeXml(toolCode,scan_list);
        FillTable(7,scan_list,4,dllGetTable,dllInsertRow,dllSetCellText);
        scan_list.clear();


        //应急处置情况
        strSql = QString("SELECT * FROM website_condition WHERE id=%1").arg(g_task_id);
        ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
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
        QMessageBox::information(this,QString("成功"),QString("生成报告成功"),
                                 QMessageBox::Yes);
    }
}








