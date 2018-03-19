#ifndef WORDDIALOG_H
#define WORDDIALOG_H

#include <QDialog>


namespace Ui {
class wordDialog;
}

class wordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit wordDialog(QWidget *parent = 0);
    ~wordDialog();
    void initLabel();

private slots:
    void on_wordButton_clicked();

private:
    Ui::wordDialog *ui;
    QString strTime;
};

#endif // WORDDIALOG_H
