#ifndef EDITRECORDDIALOG_H
#define EDITRECORDDIALOG_H

#include <QDialog>

namespace Ui {
class editRecordDialog;
}

class editRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editRecordDialog(QWidget *parent = 0);
    ~editRecordDialog();
    void setTableWidget();

private:
    Ui::editRecordDialog *ui;
};

#endif // EDITRECORDDIALOG_H
