#ifndef ADDASSETDIALOG_H
#define ADDASSETDIALOG_H

#include <QDialog>

namespace Ui {
class addAssetDialog;
}

class addAssetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addAssetDialog(QWidget *parent = 0);
    ~addAssetDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::addAssetDialog *ui;
};

#endif // ADDASSETDIALOG_H
