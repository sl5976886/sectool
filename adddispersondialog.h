#ifndef ADDDISPERSONDIALOG_H
#define ADDDISPERSONDIALOG_H

#include <QDialog>

namespace Ui {
class addDisPersonDialog;
}

class addDisPersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDisPersonDialog(QWidget *parent = 0);
    ~addDisPersonDialog();
    QPushButton *certainButton;

private:
    Ui::addDisPersonDialog *ui;

private slots:
    void certain_click();
    void on_cancelButton_clicked();
};

#endif // ADDDISPERSONDIALOG_H
