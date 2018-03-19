#include "toolsetdialog.h"
#include "ui_toolsetdialog.h"
#include "commonfunction.h"
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>

toolSetDialog::toolSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::toolSetDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(QString("工具集"));
    buttonInit();
    qInstallMessageHandler(outputMessage);
}

toolSetDialog::~toolSetDialog()
{
    delete ui;
}

void toolSetDialog::buttonInit()
{
//    QString style = QString("QPushButton{background-color:qlineargradient(spread:reflect, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(38, 117, 216, 255), stop:1 rgba(42, 156, 230, 255));color: white;border: 1px solid rgb(222,227,232);}"
//                            "QPushButton:hover{background-color:rgb(52,197,252); color: white;}"
//                            "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style: inset;}");
//    QString style1 = QString("QPushButton{background-color:rgb(240,247,253);color:black;border: 1px solid rgb(222,227,232);}"
//                             "QPushButton:hover{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(52, 194, 250, 255), stop:1 rgba(16, 144, 248, 255));color:white;}"
//                             "QPushButton:pressed{background-color:rgb(16,144,248);color:white;border-style: inset;}");
//    ui->openButton_1->setStyleSheet(style);
//    ui->openButton_2->setStyleSheet(style);
//    ui->openButton_3->setStyleSheet(style);
//    ui->openButton_4->setStyleSheet(style);
//    ui->openButton_5->setStyleSheet(style);
//    ui->openButton_6->setStyleSheet(style);
//    ui->openButton_7->setStyleSheet(style);
//    ui->openButton_8->setStyleSheet(style);
//    ui->openButton_9->setStyleSheet(style);
//    ui->openButton_10->setStyleSheet(style);
//    ui->openButton_11->setStyleSheet(style);

//    ui->manualButton_1->setStyleSheet(style1);
//    ui->manualButton_2->setStyleSheet(style1);
//    ui->manualButton_3->setStyleSheet(style1);
//    ui->manualButton_4->setStyleSheet(style1);
//    ui->manualButton_5->setStyleSheet(style1);
//    ui->manualButton_6->setStyleSheet(style1);
//    ui->manualButton_7->setStyleSheet(style1);
//    ui->manualButton_8->setStyleSheet(style1);
//    ui->manualButton_9->setStyleSheet(style1);
//    ui->manualButton_10->setStyleSheet(style1);
//    ui->manualButton_11->setStyleSheet(style1);

    QString style = QString("QPushButton{border:none;background-color:#444444;}"
                            "QPushButton:hover{border:none;background-color:#444444;}"
                            "QPushButton:pressed{border:none;background-color:#444444;}");
    ui->pushButton->setStyleSheet(style);
    ui->pushButton_2->setStyleSheet(style);
    ui->pushButton_3->setStyleSheet(style);
    ui->pushButton_4->setStyleSheet(style);
    ui->pushButton_5->setStyleSheet(style);
    ui->pushButton_6->setStyleSheet(style);
    ui->pushButton_8->setStyleSheet(style);
    ui->pushButton_9->setStyleSheet(style);
    ui->pushButton_10->setStyleSheet(style);
    ui->pushButton_11->setStyleSheet(style);
    ui->pushButton_12->setStyleSheet(style);
}

//恶意代码
void toolSetDialog::on_openButton_1_clicked()
{
    QMessageBox::information(this,QString("提示"),QString("请插入U盘运行工具"),
                             QMessageBox::Yes);
}

//SQL注入验证
void toolSetDialog::on_openButton_2_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/SQL.ExecuteFileName").toString();
    QString workPath = settings.value("onlinecheck/SQL.WorkPath").toString();
    bool ok = QProcess::startDetached(strPath,QStringList(),workPath);
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//系统漏扫
void toolSetDialog::on_openButton_3_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/netWorkLeakScan.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//网站安全
void toolSetDialog::on_openButton_4_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/web.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//数据库
void toolSetDialog::on_openButton_5_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/DB.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//主机病毒
void toolSetDialog::on_openButton_6_clicked()
{
    QMessageBox::information(this,QString("提示"),QString("请插入U盘运行工具"),
                             QMessageBox::Yes);
}

//主机木马
void toolSetDialog::on_openButton_7_clicked()
{
    QMessageBox::information(this,QString("提示"),QString("请插入U盘运行工具"),
                             QMessageBox::Yes);
}

//抓包工具
void toolSetDialog::on_openButton_8_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/Capture.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//日志分析工具
void toolSetDialog::on_openButton_9_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/Analyze.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//日志切割工具
void toolSetDialog::on_openButton_10_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/Cut.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//文件恢复工具
void toolSetDialog::on_openButton_11_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    strPath = settings.value("onlinecheck/Recover.ExecuteFileName").toString();
    bool ok = QProcess::startDetached(strPath,QStringList());
    if(!ok)
    {
        QMessageBox::warning(this,QString("失败"),QString("启动程序失败"),
                             QMessageBox::Yes);
    }
}

//恶意代码
void toolSetDialog::on_manualButton_1_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    settings.setIniCodec("utf-8");
    strPath = settings.value("offlinecheck/Eyi.ManualFile").toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(strPath));
}

//SQL注入
void toolSetDialog::on_manualButton_2_clicked()
{
    QMessageBox::warning(this,QString("抱歉"),QString("暂时没有用户手册"),
                         QMessageBox::Yes);
}

//系统漏扫
void toolSetDialog::on_manualButton_3_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    settings.setIniCodec("utf-8");
    strPath = settings.value("onlinecheck/netWorkLeakScan.ManualFile").toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(strPath));
}

//webscan
void toolSetDialog::on_manualButton_4_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    settings.setIniCodec("utf-8");
    strPath = settings.value("onlinecheck/web.ManualFile").toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(strPath));
}

//数据库
void toolSetDialog::on_manualButton_5_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    settings.setIniCodec("utf-8");
    strPath = settings.value("onlinecheck/DB.ManualFile").toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(strPath));
}

//病毒
void toolSetDialog::on_manualButton_6_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    settings.setIniCodec("utf-8");
    strPath = settings.value("offlinecheck/Bingdu.ManualFile").toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(strPath));
}

//木马
void toolSetDialog::on_manualButton_7_clicked()
{
    QString strPath;
    QString localPath = qApp->applicationDirPath();
    localPath+="\\config.ini";
    QSettings settings(localPath,QSettings::IniFormat);
    settings.setIniCodec("utf-8");
    strPath = settings.value("offlinecheck/Muma.ManualFile").toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(strPath));
}

void toolSetDialog::on_manualButton_8_clicked()
{
    QMessageBox::warning(this,QString("抱歉"),QString("暂时没有用户手册"),
                         QMessageBox::Yes);
}

void toolSetDialog::on_manualButton_9_clicked()
{
    QMessageBox::warning(this,QString("抱歉"),QString("暂时没有用户手册"),
                         QMessageBox::Yes);
}

void toolSetDialog::on_manualButton_10_clicked()
{
    QMessageBox::warning(this,QString("抱歉"),QString("暂时没有用户手册"),
                         QMessageBox::Yes);
}

void toolSetDialog::on_manualButton_11_clicked()
{
    QMessageBox::warning(this,QString("抱歉"),QString("暂时没有用户手册"),
                         QMessageBox::Yes);
}
