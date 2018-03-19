#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H

#include <time.h>
#include <QDialog>
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QMap>
#include <QVector>

void GetAsciiTime(time_t &t, char *str,const int &nLen);
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);  //写log
void operateLog(QString action,QString state);  //操作记录

extern int g_task_id;
extern QString g_user_name;
extern QString g_report_path;
extern QString g_ip;    //主机IP
extern QString g_code;  //启动的程序
extern QMap<QString,QStringList> g_ip_map;

extern QMap<QString,QStringList> g_log_map;

extern QStringList webList; //editwebdialog中所填的内容
extern QStringList secList; //editsecdialog中所填的内容
extern QStringList safeList; //editsafedialog中所填的内容

extern bool ifDispose;  //是否是添加处置人员

#endif
