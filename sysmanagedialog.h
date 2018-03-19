#ifndef SYSMANAGEDIALOG_H
#define SYSMANAGEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class sysManageDialog;
}

class sysManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sysManageDialog(QWidget *parent = 0);
    ~sysManageDialog();
    void buttonInit();

    void userTableViewInit();
    void refreshUser();
    void sysSetShow();

    char strEndTime[128];

private slots:
    void on_unitButton_clicked();

    void on_userButton_clicked();

    void on_logButton_clicked();

    void on_configButton_clicked();

    void on_newUserButton_clicked();

    void on_refreshButton_clicked();

    void on_modifyButton_1_clicked();

    void on_revokeButton_1_clicked();

    void on_deleteButton_1_clicked();

    void on_newComButton_clicked();

    void on_modifyButton_2_clicked();

    void on_revokeButton_2_clicked();

    void on_refreshButton_2_clicked();

    void on_inquiryButton_clicked();

    void on_clearButton_clicked();

    void on_exportButton_clicked();

    void on_backupButton_clicked();

    void on_cleanButton_clicked();

    void refreshUserTable();

    void refreshComTable();

    void on_saveConfigButton_clicked();

    void on_restoreConfigButton_clicked();

private:
    Ui::sysManageDialog *ui;
    QSqlTableModel *userModel;
    QSqlTableModel *comModel;
    QSqlTableModel *logModel;
    QString buttonStyle;
    QString buttonStyle_2;
};

#endif // SYSMANAGEDIALOG_H
