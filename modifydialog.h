#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QDialog>

namespace Ui {
class modifyDialog;
}

class modifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifyDialog(QWidget *parent = 0);
    ~modifyDialog();
    QPushButton *certainButton;
    void tableInit();
    void setButtonStyle();

private slots:
    void certain_click();

private:
    Ui::modifyDialog *ui;
};

#endif // MODIFYDIALOG_H
