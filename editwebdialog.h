#ifndef EDITWEBDIALOG_H
#define EDITWEBDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>

namespace Ui {
class editWebDialog;
}

class editWebDialog : public QDialog
{
    Q_OBJECT

public:
    explicit editWebDialog(QWidget *parent = 0);
    ~editWebDialog();
    void setScrollView();
    QPushButton *certainButton;
    void initRadio();
    void initDataShow();

private:
    Ui::editWebDialog *ui;
    QButtonGroup *relationGroup;
    QButtonGroup *comGroup;
    QButtonGroup *induGroup;
    QButtonGroup *dbGroup;
    QButtonGroup *djpcGroup;
    QButtonGroup *zrsGroup;
    QButtonGroup *fwlmGroup;
private slots:
    void updateDate();
};

#endif // EDITWEBDIALOG_H
