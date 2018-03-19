#ifndef NEWCOMDIALOG_H
#define NEWCOMDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class newComDialog;
}

class newComDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newComDialog(QWidget *parent = 0);
    ~newComDialog();
    QPushButton *certainButton;

private slots:
    void certain_click();

private:
    Ui::newComDialog *ui;
};

#endif // NEWCOMDIALOG_H
