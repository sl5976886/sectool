#include "webtablewidgeteditdialog.h"
#include "ui_webtablewidgeteditdialog.h"

webTableWidgetEditDialog::webTableWidgetEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::webTableWidgetEditDialog)
{
    ui->setupUi(this);
}

webTableWidgetEditDialog::~webTableWidgetEditDialog()
{
    delete ui;
}
