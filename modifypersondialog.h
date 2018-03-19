#ifndef MODIFYPERSONDIALOG_H
#define MODIFYPERSONDIALOG_H

#include <QDialog>

namespace Ui {
class modifyPersonDialog;
}

class modifyPersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifyPersonDialog(QWidget *parent = 0);
    QPushButton *certainButton;
    void getPoliceNum(QString num);
    QString mNum;
    void LineEditInit();
    ~modifyPersonDialog();

private slots:
    void certain_click();

    void on_pushButton_2_clicked();

private:
    Ui::modifyPersonDialog *ui;
};

#endif // MODIFYPERSONDIALOG_H
