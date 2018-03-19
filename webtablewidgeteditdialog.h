#ifndef WEBTABLEWIDGETEDITDIALOG_H
#define WEBTABLEWIDGETEDITDIALOG_H

#include <QDialog>

namespace Ui {
class webTableWidgetEditDialog;
}

class webTableWidgetEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit webTableWidgetEditDialog(QWidget *parent = 0);
    ~webTableWidgetEditDialog();

private:
    Ui::webTableWidgetEditDialog *ui;
};

#endif // WEBTABLEWIDGETEDITDIALOG_H
