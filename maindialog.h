#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QTimer>
#include "starttool.h"
#include "reportthread.h"

namespace Ui {
class mainDialog;
}

class mainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit mainDialog(QWidget *parent = 0);
    ~mainDialog();
    //startTool toolThread;
    void buttonInit();

private slots:
    void on_newTaskButton_clicked();

    void on_pushButton_2_clicked();


    void on_treatButton_clicked();

    void on_userButton_clicked();

    void on_openTaskButton_clicked();

    void on_webscanButton_2_clicked();

    void on_webscanButton_clicked();

    void on_wordButton_clicked();

    void connect_db_slot();

    void openTaskShow_slot();

    void on_basicModifyButton_clicked();

    void refresh_table_slot();

    void on_personAddButton_clicked();

    void on_personModifyButton_clicked();

    void on_personDelButton_clicked();

    void on_hostAddButton_clicked();

    void on_hostDelButton_clicked();

    void on_actionMinimize_triggered();     //最小化窗口
    void on_actionClose_triggered();        //关闭窗口

    void on_saveButton_clicked();

    void on_deleButton_clicked();

    void on_closeButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void log_table_show_slot(QString logIp);

    void report_success_slot(int code);

private:
    Ui::mainDialog *ui;
    QTimer *timer;

    void basicTableShow();
    void personTableShow();
    void hostTableShow();

    //自己重新实现拖动操作
    QPoint mousePosition;
    bool isMousePressed;

    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent * event);
    void initTitleBar();

    //打开word文档
    void openWordReport();
    //word报告预览
    void wordPreview();

    ReportThread reportThread;

    void initTaskState();
};

#endif // MAINDIALOG_H
