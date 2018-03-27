#ifndef NEWUSERDIALOG_H
#define NEWUSERDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class newUserDialog;
}

class newUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newUserDialog(QWidget *parent = 0);
    ~newUserDialog();
    void comBoxInit();
    QPushButton *certainButton;

private slots:
    void determineButton_slot();

private:
    Ui::newUserDialog *ui;
};

#endif // NEWUSERDIALOG_H
