#ifndef EDITSAFEDIALOG_H
#define EDITSAFEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class editSafeDialog;
}

class editSafeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editSafeDialog(QWidget *parent = 0);
    ~editSafeDialog();
    void setScrollView();
    QPushButton *certainButton;
    void initRadio();

private:
    Ui::editSafeDialog *ui;
    QButtonGroup *attackGroup;
    QButtonGroup *passGroup;
    QButtonGroup *tamperGroup;
    QButtonGroup *hostGroup;
    QButtonGroup *antiGroup;
    QButtonGroup *serverGroup;
    QButtonGroup *updateGroup;
    QButtonGroup *existGroup;
    QButtonGroup *beifenGroup;
    QButtonGroup *fangshiGroup;
    QButtonGroup *shenjiGroup;
    QButtonGroup *aqxgGroup;
    QButtonGroup *sjbbGroup;
    QButtonGroup *sjbhGroup;
    QButtonGroup *dwfwGroup;
    QButtonGroup *aqfhGroup;
    QButtonGroup *dsfcjGroup;
    QButtonGroup *dsfldGroup;
    QButtonGroup *htglGroup;
    QButtonGroup *ycljGroup;
    QButtonGroup *mrklGroup;
    QButtonGroup *bdgxGroup;
    QButtonGroup *bdgxplGroup;
    QButtonGroup *stcsGroup;
    QButtonGroup *jxplGroup;
    QButtonGroup *cdnGroup;

private slots:
    void certain_click();
};

#endif // EDITSAFEDIALOG_H
