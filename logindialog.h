#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "registerdialog.h"
#include "forgetdialog.h"
#include "maindialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    bool isLocked();
    void initStyle();

private slots:
    void on_pushButton_clicked();

    void on_LoginPushButton_clicked();

    void on_forgetPushButton_clicked();

private:
    Ui::LoginDialog *ui;
    mainDialog *mw;
};


#endif // LOGINDIALOG_H
