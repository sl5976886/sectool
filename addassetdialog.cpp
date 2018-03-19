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
    this->setWindowTitle(QString::fromLocal8Bit("����ʲ�"));
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
        QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("�ʲ����Ʋ���Ϊ�գ�"));
        ui->nameLineEdit->setFocus();
    }
    else if(ui->IPLineEdit->text().length()==0)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("IP��ַ����Ϊ�գ�"));
        ui->IPLineEdit->setFocus();
    }
    else
    {
        QMessageBox::information(this,QString::fromLocal8Bit("�ɹ�"),QString::fromLocal8Bit("�ʲ���ӳɹ���"));
        this->hide();
    }
}
