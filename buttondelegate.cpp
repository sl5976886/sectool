#include "buttondelegate.h"
#include "commonfunction.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QFileDialog>
#include <QDomDocument>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QAbstractButton>


ButtonDelegate::ButtonDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton* button = m_btns.value(index);
    if (!button) {
        button = new QStyleOptionButton();
        button->rect = option.rect.adjusted(10, 4, -10, -4);
        button->text = QString("启动");
        button->state |= QStyle::State_Enabled;

        (const_cast<ButtonDelegate *>(this))->m_btns.insert(index, button);
    }
    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());

    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);


}

QString toolCode(QString toolName)
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

bool isDirExist(QString fullPath)
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

void startTool(QString code)
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

    if(strPath!=QString("nocode"))
    {
       bool ok = QProcess::startDetached(strPath,QStringList());
       if(!ok)
       {
           qWarning()<<"start process fail:"<<strPath;
       }
    }
}

QString xmlPath(QString code)
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


bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(10, 4, -10, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
            m_btns.value(index)->state |= QStyle::State_Sunken;
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(10, 4, -10, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
            m_btns.value(index)->state &= (~QStyle::State_Sunken);

//            int currentValue = index.model()->data(index,Qt::DisplayRole).toInt();
//            qDebug()<<"current value:"<<currentValue;
            QString strIp = model->data(model->index(index.row(),1), Qt::DisplayRole).toString();
            g_ip = strIp;
            QString strName = model->data(model->index(index.row(),2), Qt::DisplayRole).toString();
            QString strType = model->data(model->index(index.row(),3), Qt::DisplayRole).toString();
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

            QString strPath = settings.value("autoscan/OnlineScanDir").toString();
            strPath+="\\";

            QString strId = QString("%1").arg(g_task_id);
            QString strCode = toolCode(strName);

            //SQL注入验证工具不接受任务xml，所以直接启动
            if(strCode==QString("010"))
            {
                QString mPath = settings.value("onlinecheck/SQL.ExecuteFileName").toString();
                QString workPath = settings.value("onlinecheck/SQL.WorkPath").toString();
                bool ok = QProcess::startDetached(mPath,QStringList(),workPath);
                if(!ok)
                {
                    QString strTitle = QString("提示");
                    QString strMsg = QString("启动工具失败");
                    QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, strMsg, QMessageBox::Ok, 0);
                    if(NULL!=WrrMsg->button(QMessageBox::Ok))
                    {
                        WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
                    }
                    WrrMsg->exec();
                    return false;
                }
                return true;
            }

            g_code = strCode;
            strPath+=strId;
            strPath+="\\";
            g_report_path = strPath;
            strPath+=strIp;
            strPath+="\\";
            strPath+=strCode;
            isDirExist(strPath);

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
                QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, strMsg, QMessageBox::Ok, 0);
                if(NULL!=WrrMsg->button(QMessageBox::Ok))

                {

                    WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));

                }
                WrrMsg->exec();
                return true;
            }
            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
            {
                qWarning()<<"open file fail ";
            }
            QTextStream out(&file);
            out.setCodec("UTF-8");
            doc.save(out,4,QDomNode::EncodingFromTextStream);
            file.close();

            startTool(strCode);

            QString action = QString("漏洞验证");
            QString state = QString("成功");
            operateLog(action,state);
            QSqlQuery query;
            QString strSql = QString("UPDATE check_task SET operate_state=1 WHERE id=%1 AND run_tool='%2'")
                    .arg(g_task_id).arg(strName);
            bool ok = query.exec(strSql);
            if(!ok)
            {
                qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
            }
        }
    }
    return true;
}
