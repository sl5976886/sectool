#include "modifypersondialog.h"
#include "ui_modifypersondialog.h"
#include "commonfunction.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

modifyPersonDialog::modifyPersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifyPersonDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("修改应急处置人员"));
    this->setFixedSize(this->width(),this->height());
    ui->numLineEdit->setFocusPolicy(Qt::NoFocus);

    certainButton = new QPushButton(QString("确定"),this);
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);
    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(certain_click()));
}

modifyPersonDialog::~modifyPersonDialog()
{
    delete ui;
}

void modifyPersonDialog::getPoliceNum(QString num)
{
    mNum = num;
}

void modifyPersonDialog::LineEditInit()
{
    QSqlQuery query;
    QString strSql = QString("SELECT * FROM dispose_person WHERE task_id=%1 AND police_num='%2'").arg(g_task_id).arg(mNum);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
        return;
    }
    if(query.next())
    {
        QString strName = query.value(1).toString();
        QString strNum = query.value(2).toString();
        QString strPhone = query.value(4).toString();
        QString strDuty = query.value(5).toString();

        ui->nameLineEdit->setText(strName);
        ui->numLineEdit->setText(strNum);
        ui->phoneLineEdit->setText(strPhone);
        ui->positionLineEdit->setText(strDuty);
    }
}

void modifyPersonDialog::certain_click()
{
    if(ui->nameLineEdit->text().length()==0)
    {
        QString strTitle = QString("警告");
        QString strMsg = QString("姓名不能为空");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->nameLineEdit->setFocus();
    }
    else
    {
        QString strName = ui->nameLineEdit->text();
        QString strPhone = ui->phoneLineEdit->text();
        QString strDuty = ui->positionLineEdit->text();
        QString strNum = ui->numLineEdit->text();
        QString strSql = QString("UPDATE dispose_person SET name='%1',phone='%2',position='%3' WHERE police_num='%4' AND "
                                 "task_id=%5").arg(strName,strPhone,strDuty,strNum).arg(g_task_id);

        QSqlQuery query;
        bool ok = query.exec(strSql);
        if(!ok)
        {
            qWarning()<<"sql error: "<<query.lastError()<<"sql is: "<<strSql.toStdString().c_str();
            return;
        }

        QString strTitle = QString("成功");
        QString strMsg = QString("修改已保存");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        this->close();
    }
}

void modifyPersonDialog::on_pushButton_2_clicked()
{
    this->close();
}
