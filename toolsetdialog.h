#ifndef TOOLSETDIALOG_H
#define TOOLSETDIALOG_H

#include <QDialog>

namespace Ui {
class toolSetDialog;
}

class toolSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit toolSetDialog(QWidget *parent = 0);
    ~toolSetDialog();
    void buttonInit();

private slots:
    void on_openButton_1_clicked();

    void on_openButton_2_clicked();

    void on_openButton_3_clicked();

    void on_openButton_4_clicked();

    void on_openButton_5_clicked();

    void on_openButton_6_clicked();

    void on_openButton_7_clicked();

    void on_openButton_8_clicked();

    void on_openButton_9_clicked();

    void on_openButton_10_clicked();

    void on_openButton_11_clicked();

    void on_manualButton_1_clicked();

    void on_manualButton_2_clicked();

    void on_manualButton_3_clicked();

    void on_manualButton_4_clicked();

    void on_manualButton_5_clicked();

    void on_manualButton_6_clicked();

    void on_manualButton_7_clicked();

    void on_manualButton_8_clicked();

    void on_manualButton_9_clicked();

    void on_manualButton_10_clicked();

    void on_manualButton_11_clicked();

private:
    Ui::toolSetDialog *ui;
};

#endif // TOOLSETDIALOG_H
