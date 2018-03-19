#ifndef ADDHOSTDIALOG_H
#define ADDHOSTDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <QPushButton>

namespace Ui {
class addHostDialog;
}

class addHostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addHostDialog(QWidget *parent = 0);
    ~addHostDialog();
    void radioButtonInit();
    QPushButton *certainButton;

private slots:
    void certain_click();

private:
    Ui::addHostDialog *ui;
    QButtonGroup *kindGroup;
    QButtonGroup *sysGroup;
    QButtonGroup *dataGroup;
    QButtonGroup *middleGroup;
    QButtonGroup *toolGroup;
    void infoInit();
};

#endif // ADDHOSTDIALOG_H
