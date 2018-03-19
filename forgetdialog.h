#ifndef FORGETDIALOG_H
#define FORGETDIALOG_H

#include <QDialog>

namespace Ui {
class ForgetDialog;
}

class ForgetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForgetDialog(QWidget *parent = 0);
    ~ForgetDialog();

private slots:
    void on_okButton_clicked();

private:
    Ui::ForgetDialog *ui;
};

#endif // FORGETDIALOG_H
