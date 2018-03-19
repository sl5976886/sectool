#include "commonfunction.h"
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

//获取本地时间
void GetAsciiTime(time_t &t, char *str,const int &nLen)
{
    strftime( str, nLen, "%Y/%m/%d %X ",localtime(&t) );
}


void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    default:
        text = QString("Debug:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QString localPath = qApp->applicationDirPath();
    localPath+="\\log.txt";
    QFile file(localPath);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

void operateLog(QString action, QString state)
{
    QSqlQuery query;
    qInstallMessageHandler(outputMessage);
    QString strSql = QString("INSERT INTO operate_log(user_name,action,state) VALUES('%1','%2','%3')")
            .arg(g_user_name).arg(action).arg(state);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
    }
}


int g_task_id = 0;

QString g_user_name = QString("");
QString g_report_path = QString("");
QString g_ip = QString("");
QString g_code = QString("");
QMap<QString,QStringList> g_ip_map;

QStringList webList;
QStringList secList;
QStringList safeList;

QMap<QString,QStringList> g_log_map;

bool ifDispose = false;

