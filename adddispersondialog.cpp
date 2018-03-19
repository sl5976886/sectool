#include "adddispersondialog.h"
#include "ui_adddispersondialog.h"
#include "commonfunction.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QHBoxLayout>

addDisPersonDialog::addDisPersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDisPersonDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(QString("添加应急处置人员"));
    qInstallMessageHandler(outputMessage);

    certainButton = new QPushButton(QString("确定"),this);
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);

    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(certain_click()));

    QSqlQuery query;
    QString strSql = QString("SELECT ORGCODE FROM register_user WHERE user_name='%1'").arg(g_user_name);
    QString orgCode;
    bool ok = query.exec(strSql);
    if(ok)
    {
        if(query.next())
        {
            orgCode = query.value(0).toString();
            orgCode = orgCode.mid(0,2);
        }
    }
    else
    {
        qWarning()<<"sql err:"<<query.lastError();
        QString strTitle = QString("错误");
        QString strMsg = QString("获取单位信息失败，请检查数据库连接");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
    }

    strSql = QString("SELECT ORGNAME FROM cfg_orgcode WHERE ORGCODE LIKE '%1%'").arg(orgCode);
    query.exec(strSql);
    ui->comboBox->setFixedHeight(20);
    while(query.next())
    {
        QString name = query.value(0).toString();
        QPixmap pixmap(1, ui->comboBox->height());
        pixmap.fill(Qt::transparent);
        QIcon icon(pixmap);
        ui->comboBox->setIconSize(QSize(1,ui->comboBox->height()));
        ui->comboBox->addItem(icon,name);
    }
}

addDisPersonDialog::~addDisPersonDialog()
{
    delete ui;
}

void addDisPersonDialog::certain_click()
{
    QString strName = ui->nameLineEdit->text();
    QString strPolice = ui->pliceNumLineEdit->text();
    if(strName.isEmpty())
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请填写姓名");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->nameLineEdit->setFocus();
        return;
    }
    if(strPolice.isEmpty())
    {
        QString strTitle = QString("错误");
        QString strMsg = QString("请填写警号");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        ui->pliceNumLineEdit->setFocus();
        return;
    }
    QString strCom = ui->comboBox->currentText();
    QString strPhone = ui->phoneLineEdit->text();
    QString strDuty = ui->dutyLineEdit->text();

    QSqlQuery query;
    QString strSql = QString("INSERT INTO dispose_person SET name='%1',police_num='%2',check_company='%3',"
                             "phone='%4',position='%5',task_id=%6").arg(strName).arg(strPolice).arg(strCom)
            .arg(strPhone).arg(strDuty).arg(g_task_id);
    bool ok = query.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql err:"<<query.lastError();
        QString strTitle = QString("添加失败");
        QString strMsg = QString("数据库连接失败");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        return;
    }
    else
    {
        QString strTitle = QString("添加成功");
        QString strMsg = QString("添加应急处置人员成功");
        QString showMsg = "<font color='black'>"+strMsg+"</font>";
        QMessageBox *WrrMsg = new QMessageBox(QMessageBox::NoIcon, strTitle, showMsg, QMessageBox::Ok, this);
        if(NULL!=WrrMsg->button(QMessageBox::Ok))
        {
            WrrMsg->button(QMessageBox::Ok)->setText(QString("确定"));
        }
        WrrMsg->exec();
        this->close();
        return;
    }
}

void addDisPersonDialog::on_cancelButton_clicked()
{
    this->close();
    return;
}
