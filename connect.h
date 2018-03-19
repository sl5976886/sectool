#ifndef CONNECT_H
#define CONNECT_H
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSettings>
#include "commonfunction.h"

static bool createConnect()
{
    qInstallMessageHandler(outputMessage);
    // 打开MySQL
    // mysql lib下的libmysql.dll copy 到Qt \bin下
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("sectool");
    db.setUserName("sectool");
    db.setPassword("`1q`1q");

    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    int mPort = settings.value("database/database.port").toInt();

    db.setPort(mPort);
    if (!db.open())
    {
        QMessageBox::critical(0, "Cannot open database",
                              "Unable to establisha database connection.",
                              QMessageBox::Cancel);
        qWarning()<<"database error:"<<db.lastError();
        return false;
    }
    else
    {
        qDebug()<<"connect success";
        return true;
    }
}

#endif // CONNECT_H
