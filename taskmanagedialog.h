#ifndef TASKMANAGEDIALOG_H
#define TASKMANAGEDIALOG_H

#include "maindialog.h"
#include <QDialog>
#include <QSqlTableModel>
#include <QMouseEvent>

namespace Ui {
class taskManageDialog;
}

class taskManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit taskManageDialog(QWidget *parent = 0);
    ~taskManageDialog();
    void taskTableViewInit();
    void refreshTableView();
    void hookMainDialog(mainDialog *w);
    mainDialog *mmw;

private slots:
    void on_newButton_clicked();

    void on_openButton_clicked();

    void on_deleteButton_clicked();

    void on_closeButton_clicked();

    //void on_closeButton_2_clicked();

    void on_saveButton_clicked();

    void on_modifyButton_clicked();

    void other_refresh();

    void task_fresh_slot();
signals:
    void openTask();

private:
    Ui::taskManageDialog *ui;
    QSqlTableModel *taskTableModel;
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // TASKMANAGEDIALOG_H
