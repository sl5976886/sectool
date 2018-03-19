#ifndef EDITSECDIALOG_H
#define EDITSECDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class editSecDialog;
}

class editSecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editSecDialog(QWidget *parent = 0);
    ~editSecDialog();
    void setScrollBar();
    QPushButton *certainButton;
    void initRadio();

private:
    Ui::editSecDialog *ui;
    QButtonGroup *platformGroup;
    QButtonGroup *oneGroup;
    QButtonGroup *twoGroup;
    QButtonGroup *threeGroup;
    QButtonGroup *fourGroup;
    QButtonGroup *fiveGroup;
    QButtonGroup *sixGroup;
    QButtonGroup *sevenGroup;
    QButtonGroup *dataGroup_1;
    QButtonGroup *dataGroup_2;
    QButtonGroup *dataGroup_3;
    QButtonGroup *dataGroup_4;
    QButtonGroup *dataGroup_5;
    QButtonGroup *dataGroup_6;
    QButtonGroup *dataGroup_7;
    QButtonGroup *dataGroup_8;

private slots:
    void certain_click();
};

#endif // EDITSECDIALOG_H
