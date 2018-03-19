#include "addassetdialog.h"
#include "ui_addassetdialog.h"
#include "commonfunction.h"
#include <QButtonGroup>
#include <QMessageBox>

addAssetDialog::addAssetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addAssetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("添加资产"));
    this->setFixedSize(this->width(),this->height());
    qInstallMessageHandler(outputMessage);

    QButtonGroup *m_assetButtonGroup = new QButtonGroup(this);
    m_assetButtonGroup->addButton(ui->saveRadioButton);
    m_assetButtonGroup->addButton(ui->boudaryRadioButton);
    m_assetButtonGroup->addButton(ui->hostRadioButton);
    m_assetButtonGroup->setExclusive(true);

    QButtonGroup *m_systemButtonGroup = new QButtonGroup(this);
    m_systemButtonGroup->addButton(ui->linuxRadioButton);
    m_systemButtonGroup->addButton(ui->windowsRadioButton);
    m_systemButtonGroup->setExclusive(true);
}

addAssetDialog::~addAssetDialog()
{
    delete ui;
}

void addAssetDialog::on_pushButton_clicked()
{
    if(ui->nameLineEdit->text().length()==0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("资产名称不能为空！"));
        ui->nameLineEdit->setFocus();
    }
    else if(ui->IPLineEdit->text().length()==0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("IP地址不能为空！"));
        ui->IPLineEdit->setFocus();
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("成功"),QString::fromLocal8Bit("资产添加成功！"));
        this->hide();
    }
}
