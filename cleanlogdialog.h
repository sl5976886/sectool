#ifndef CLEANLOGDIALOG_H
#define CLEANLOGDIALOG_H

#include <QDialog>

namespace Ui {
class cleanLogDialog;
}

class cleanLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cleanLogDialog(QWidget *parent = 0);
    ~cleanLogDialog();

private slots:
    void on_certainButton_clicked();

private:
    Ui::cleanLogDialog *ui;
};

#endif // CLEANLOGDIALOG_H
