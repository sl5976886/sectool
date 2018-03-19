#include "editrecorddialog.h"
#include "ui_editrecorddialog.h"
#include <QButtonGroup>
#include <QRadioButton>

editRecordDialog::editRecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editRecordDialog)
{
    ui->setupUi(this);
}

editRecordDialog::~editRecordDialog()
{
    delete ui;
}

void editRecordDialog::setTableWidget()
{
    ui->webTableWidget->setSpan(1,1,1,3);
    ui->webTableWidget->setSpan(5,1,1,3);
    ui->webTableWidget->setSpan(7,1,1,3);
    ui->webTableWidget->setSpan(8,1,1,3);
    ui->webTableWidget->setSpan(9,1,1,3);
    ui->webTableWidget->setSpan(10,1,1,3);
    ui->webTableWidget->setSpan(11,1,1,3);
    ui->webTableWidget->setSpan(12,1,1,3);
    ui->webTableWidget->setSpan(13,1,1,3);
    ui->webTableWidget->setRowHeight(7,50);
    ui->webTableWidget->setRowHeight(7,60);

//    QButtonGroup *bg = new QButtonGroup(this);
}
