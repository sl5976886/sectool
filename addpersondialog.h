#ifndef ADDPERSONDIALOG_H
#define ADDPERSONDIALOG_H

#include <QDialog>

namespace Ui {
class addPersonDialog;
}

class addPersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addPersonDialog(QWidget *parent = 0);
    ~addPersonDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::addPersonDialog *ui;
};

#endif // ADDPERSONDIALOG_H
