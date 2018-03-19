#ifndef DISPOSEDIALOG_H
#define DISPOSEDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "renderthread.h"
#include "maindialog.h"

namespace Ui {
class disposeDialog;
}

class disposeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit disposeDialog(QWidget *parent = 0);
    ~disposeDialog();
    void safeTableWidgetShow();
    void webTableWidgetShow();
    void archTableWidgetShow();
    void attackTableWidgetShow();
    void anaTableWidgetShow();
    void eventTableWidgetShow();
    void disposeTableWidgetShow();
    void antiTableWidgetShow();
    void setScrollBarSheet();
    void safeTableData();
    void webTableData();
    void archTableData();
    void antiTableData();
    QString toolCode(QString toolName);
    void startTool(QString code);
    QString xmlPath(QString code);
    bool isDirExist(QString fullPath);
    void disposeShow();

    void webDataUpdate();

    QPushButton *autoButton;
    QPushButton *manualButton;
    QPushButton *anaButton_1;
    QPushButton *anaButton_2;
    QTextEdit *edit;
    QTextEdit *edit_2;
    QTextEdit *edit_3;
    QPushButton *btn_1;
    QPushButton *btn_2;

    QDateEdit *date_1;
    QDateEdit *date_2;

    int flag;
    int attcount;

    mainDialog *mmw;
    void hookMainDialog(mainDialog *w);

private:
    Ui::disposeDialog *ui;
    QString strPath;
    RenderThread render;
    int row;
    QTimer *timer;
    QTimer *timer_2;
    QNetworkAccessManager *send;

    QString logIp;

signals:
    void addLogToReport(QString logIp);
    void checkReport();

private slots:
    void chooseFirstWidgetSlot();
    void chooseSecondWidgetSlot();
    void on_webEditButton_clicked();
    void on_secEditButton_clicked();
    void on_antiEditButton_clicked();
    void on_addHostButton_clicked();
    void on_runToolButton_clicked();
    void on_deleHostButton_clicked();
    void on_recordButton_clicked();
    void on_recordButton_2_clicked();
    void refresh_tableWidget(QString id);
    void refresh_safeTable();
    void refresh_attackTable();
    void on_saveButton_clicked();
    void on_checkResButton_clicked();
    void refresh_secTable();
    void refresh_antiTable();
    void on_analyzeButton_clicked();
    void checkToolStart();
    void on_chooseButton_clicked();
    void checkResultSlot();
    void on_checkLogButton_clicked();

    void finisgedSlot(QNetworkReply* reply);

    void logTableShow();
    void on_logReturnButton_clicked();
    void on_addReportButton_clicked();
};

#endif // DISPOSEDIALOG_H
