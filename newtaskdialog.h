#ifndef NEWTASKDIALOG_H
#define NEWTASKDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include "maindialog.h"
#include "taskmanagedialog.h"

namespace Ui {
class newTaskDialog;
}

class newTaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newTaskDialog(QWidget *parent = 0);
    ~newTaskDialog();
    QString saveFileName;
    void hookMainDialog(mainDialog *w);
    mainDialog *mmw;
    taskManageDialog *tmm;
    void hookTaskManage(taskManageDialog *w);

private:
    Ui::newTaskDialog *ui;

private slots:
    void saveReportSlot();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
signals:
    void openTask();
};

#endif // NEWTASKDIALOG_H
