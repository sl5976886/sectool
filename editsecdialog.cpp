#include "editsecdialog.h"
#include "ui_editsecdialog.h"
#include "commonfunction.h"
#include <QScrollBar>
#include <QLayout>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

editSecDialog::editSecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editSecDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(QString("编辑记录单"));
    qInstallMessageHandler(outputMessage);

    certainButton = new QPushButton(QString("确定"),this);
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);
    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(certain_click()));

    //setScrollBar();
    initRadio();
}

editSecDialog::~editSecDialog()
{
    delete ui;
}

void editSecDialog::certain_click()
{
    secList.clear();
    QString strLanguage = QString("");
    if(ui->languageCheckBox_1->isChecked())
    {
        strLanguage += "JSP,";
    }
    if(ui->languageCheckBox_2->isChecked())
    {
        strLanguage += "PHP,";
    }
    if(ui->languageCheckBox_3->isChecked())
    {
        strLanguage += "HTML,";
    }
    if(ui->languageCheckBox_4->isChecked())
    {
        strLanguage += "ASP.NET,";
    }
    if(ui->languageCheckBox_5->isChecked())
    {
        strLanguage += "ASP,";
    }
    if(ui->languageCheckBox_6->isChecked())
    {
        strLanguage += ui->languageLineEdit->text();
    }
    secList.append(strLanguage);

    QString strPlat = QString("");
    quint16 plat_id = platformGroup->checkedId();
    switch (plat_id)
    {
    case 1:
        strPlat += QString("自行开发平台");
        break;
    case 2:
        strPlat += QString("TRS");
        break;
    case 3:
        strPlat += QString("织梦CMS");
        break;
    case 4:
        strPlat += ui->platformLineEdit->text();
        break;
    default:
        strPlat += "";
        break;
    }
    secList.append(strPlat);

    QString strMiddle = QString("");
    quint16 middle_id;
    if(ui->middleCheckBox_1->isChecked())
    {
        strMiddle += QString("其他，版本：");
        strMiddle += ui->middleLineEdit_1->text();
        strMiddle += QString("，是否使用默认管理口令：");
        middle_id = oneGroup->checkedId();
        switch (middle_id)
        {
        case 1:
            strMiddle += QString("是 ");
            break;
        case 2:
            strMiddle += QString("否 ");
            break;
        case 3:
            strMiddle += QString("无远程管理接口 ");
            break;
        default:
            strMiddle += QString("未填写 ");
            break;
        }
    }
    if(ui->middleCheckBox_2->isChecked())
    {
        strMiddle += QString("IIS，版本：");
        strMiddle += ui->middleLineEdit_2->text();
        strMiddle += QString("，是否使用默认管理口令：");
        middle_id = twoGroup->checkedId();
        switch (middle_id)
        {
        case 1:
            strMiddle += QString("是 ");
            break;
        case 2:
            strMiddle += QString("否 ");
            break;
        case 3:
            strMiddle += QString("无远程管理接口 ");
            break;
        default:
            strMiddle += QString("未填写 ");
            break;
        }
    }
    if(ui->middleCheckBox_3->isChecked())
    {
        strMiddle += QString("Apache，版本：");
        strMiddle += ui->middleLineEdit_3->text();
        strMiddle += QString("，是否使用默认管理口令：");
        middle_id = threeGroup->checkedId();
        switch (middle_id)
        {
        case 1:
            strMiddle += QString("是 ");
            break;
        case 2:
            strMiddle += QString("否 ");
            break;
        case 3:
            strMiddle += QString("无远程管理接口 ");
            break;
        default:
            strMiddle += QString("未填写 ");
            break;
        }
    }
    if(ui->middleCheckBox_4->isChecked())
    {
        strMiddle += QString("Tomcat，版本：");
        strMiddle += ui->middleLineEdit_4->text();
        strMiddle += QString("，是否使用默认管理口令：");
        middle_id = fourGroup->checkedId();
        switch (middle_id)
        {
        case 1:
            strMiddle += QString("是 ");
            break;
        case 2:
            strMiddle += QString("否 ");
            break;
        case 3:
            strMiddle += QString("无远程管理接口 ");
            break;
        default:
            strMiddle += QString("未填写 ");
            break;
        }
    }
    if(ui->middleCheckBox_5->isChecked())
    {
        strMiddle += QString("Jboss，版本：");
        strMiddle += ui->middleLineEdit_5->text();
        strMiddle += QString("，是否使用默认管理口令：");
        middle_id = fiveGroup->checkedId();
        switch (middle_id)
        {
        case 1:
            strMiddle += QString("是 ");
            break;
        case 2:
            strMiddle += QString("否 ");
            break;
        case 3:
            strMiddle += QString("无远程管理接口 ");
            break;
        default:
            strMiddle += QString("未填写 ");
            break;
        }
    }
    if(ui->middleCheckBox_6->isChecked())
    {
        strMiddle += QString("Websphere，版本：");
        strMiddle += ui->middleLineEdit_6->text();
        strMiddle += QString("，是否使用默认管理口令：");
        middle_id = sixGroup->checkedId();
        switch (middle_id)
        {
        case 1:
            strMiddle += QString("是 ");
            break;
        case 2:
            strMiddle += QString("否 ");
            break;
        case 3:
            strMiddle += QString("无远程管理接口 ");
            break;
        default:
            strMiddle += QString("未填写 ");
            break;
        }
    }
    if(ui->middleCheckBox_7->isChecked())
    {
        strMiddle += QString("Weblogic，版本：");
        strMiddle += ui->middleLineEdit_7->text();
        strMiddle += QString("，是否使用默认管理口令：");
        middle_id = sevenGroup->checkedId();
        switch (middle_id)
        {
        case 1:
            strMiddle += QString("是 ");
            break;
        case 2:
            strMiddle += QString("否 ");
            break;
        case 3:
            strMiddle += QString("无远程管理接口 ");
            break;
        default:
            strMiddle += QString("未填写 ");
            break;
        }
    }
    secList.append(strMiddle);

    QString strData = QString("");
    quint16 data_id;
    if(ui->dataCheckBox_1->isChecked())
    {
        strData += QString("MySQL，版本：");
        strData += ui->dataLineEdit_1->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_1->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    if(ui->dataCheckBox_2->isChecked())
    {
        strData += QString("SQL Server，版本：");
        strData += ui->dataLineEdit_2->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_2->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    if(ui->dataCheckBox_3->isChecked())
    {
        strData += QString("Qracle，版本：");
        strData += ui->dataLineEdit_3->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_3->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    if(ui->dataCheckBox_4->isChecked())
    {
        strData += QString("Sybase，版本：");
        strData += ui->dataLineEdit_4->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_4->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    if(ui->dataCheckBox_5->isChecked())
    {
        strData += QString("DB2，版本：");
        strData += ui->dataLineEdit_5->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_5->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    if(ui->dataCheckBox_6->isChecked())
    {
        strData += QString("MSDE，版本：");
        strData += ui->dataLineEdit_6->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_6->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    if(ui->dataCheckBox_7->isChecked())
    {
        strData += QString("Access，版本：");
        strData += ui->dataLineEdit_7->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_7->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    if(ui->dataCheckBox_8->isChecked())
    {
        strData += QString("其他，版本：");
        strData += ui->dataLineEdit_8->text();
        strData += QString("，是否使用默认用户口令：");
        data_id = dataGroup_8->checkedId();
        switch (data_id)
        {
        case 1:
            strData += QString("是 ");
            break;
        case 2:
            strData += QString("否 ");
            break;
        default:
            strData += QString("未填写 ");
            break;
        }
    }
    secList.append(strData);

    QString strServer = QString("");
    if(ui->serverCheckBox_1->isChecked())
    {
        strServer += QString("Windows 2000 Server，版本：");
        strServer += ui->serverLineEdit_1->text();
        strServer += " ";
    }
    if(ui->serverCheckBox_2->isChecked())
    {
        strServer += QString("Windows 2003 Server，版本：");
        strServer += ui->serverLineEdit_2->text();
        strServer += " ";
    }
    if(ui->serverCheckBox_3->isChecked())
    {
        strServer += QString("Windows 2008 Server，版本：");
        strServer += ui->serverLineEdit_3->text();
        strServer += " ";
    }
    if(ui->serverCheckBox_4->isChecked())
    {
        strServer += QString("Windows 2012 Server，版本：");
        strServer += ui->serverLineEdit_4->text();
        strServer += " ";
    }
    if(ui->serverCheckBox_5->isChecked())
    {
        strServer += QString("Windows 2016 Server，版本：");
        strServer += ui->serverLineEdit_5->text();
        strServer += " ";
    }
    if(ui->serverCheckBox_6->isChecked())
    {
        strServer += QString("Linux，版本：");
        strServer += ui->serverLineEdit_6->text();
        strServer += " ";
    }
    if(ui->serverCheckBox_7->isChecked())
    {
        strServer += QString("其他，版本：");
        strServer += ui->serverLineEdit_7->text();
        strServer += " ";
    }
    secList.append(strServer);

    QString strYcwh = QString("");
    if(ui->ycCheckBox_1->isChecked())
    {
        strYcwh += QString("远程桌面，版本：");
        strYcwh += ui->ycLineEdit_1->text();
        strYcwh += " ";
    }
    if(ui->ycCheckBox_2->isChecked())
    {
        strYcwh += QString("TeamViewer，版本：");
        strYcwh += ui->ycLineEdit_2->text();
        strYcwh += " ";
    }
    if(ui->ycCheckBox_3->isChecked())
    {
        strYcwh += QString("putty，版本：");
        strYcwh += ui->ycLineEdit_3->text();
        strYcwh += " ";
    }
    if(ui->ycCheckBox_4->isChecked())
    {
        strYcwh += QString("WinSCP，版本：");
        strYcwh += ui->ycLineEdit_4->text();
        strYcwh += " ";
    }
    if(ui->ycCheckBox_5->isChecked())
    {
        strYcwh += QString("其他，版本：");
        strYcwh += ui->ycLineEdit_5->text();
        strYcwh += " ";
    }
    secList.append(strYcwh);
    this->close();
}

void editSecDialog::initRadio()
{
//    QSqlQuery query;
//    QString strSql = QString("SELECT * FROM web_record_form WHERE id=%1").arg(g_task_id);
//    bool ok = query.exec(strSql);
//    if(!ok)
//    {
//        qWarning()<<"sql error: "<<query.lastError();
//    }
//    if(query.next())
//    {
//    }

    platformGroup = new QButtonGroup(this);
    platformGroup->setExclusive(true);
    platformGroup->addButton(ui->platformRadio_1,1);
    platformGroup->addButton(ui->platformRadio_2,2);
    platformGroup->addButton(ui->platformRadio_3,3);
    platformGroup->addButton(ui->platformRadio_4,4);

    oneGroup = new QButtonGroup(this);
    oneGroup->setExclusive(true);
    oneGroup->addButton(ui->oneRadio_1,1);
    oneGroup->addButton(ui->oneRadio_2,2);
    oneGroup->addButton(ui->oneRadio_3,3);

    twoGroup = new QButtonGroup(this);
    twoGroup->setExclusive(true);
    twoGroup->addButton(ui->twoRadio_1,1);
    twoGroup->addButton(ui->twoRadio_2,2);
    twoGroup->addButton(ui->twoRadio_3,3);

    threeGroup = new QButtonGroup(this);
    threeGroup->setExclusive(true);
    threeGroup->addButton(ui->threeRadio_1,1);
    threeGroup->addButton(ui->threeRadio_2,2);
    threeGroup->addButton(ui->threeRadio_3,3);

    fourGroup = new QButtonGroup(this);
    fourGroup->setExclusive(true);
    fourGroup->addButton(ui->fourRadio_1,1);
    fourGroup->addButton(ui->fourRadio_2,2);
    fourGroup->addButton(ui->fourRadio_3,3);

    fiveGroup = new QButtonGroup(this);
    fiveGroup->setExclusive(true);
    fiveGroup->addButton(ui->fiveRadio_1,1);
    fiveGroup->addButton(ui->fiveRadio_2,2);
    fiveGroup->addButton(ui->fiveRadio_3,3);

    sixGroup = new QButtonGroup(this);
    sixGroup->setExclusive(true);
    sixGroup->addButton(ui->sixRadio_1,1);
    sixGroup->addButton(ui->sixRadio_2,2);
    sixGroup->addButton(ui->sixRadio_3,3);

    sevenGroup = new QButtonGroup(this);
    sevenGroup->setExclusive(true);
    sevenGroup->addButton(ui->sevenRadio_1,1);
    sevenGroup->addButton(ui->sevenRadio_2,2);
    sevenGroup->addButton(ui->sevenRadio_3,3);

    dataGroup_1 = new QButtonGroup(this);
    dataGroup_1->setExclusive(true);
    dataGroup_1->addButton(ui->dataYesRadio_1,1);
    dataGroup_1->addButton(ui->dataNoRadio_1,2);

    dataGroup_2 = new QButtonGroup(this);
    dataGroup_2->setExclusive(true);
    dataGroup_2->addButton(ui->dataYesRadio_2,1);
    dataGroup_2->addButton(ui->dataNoRadio_2,2);

    dataGroup_3 = new QButtonGroup(this);
    dataGroup_3->setExclusive(true);
    dataGroup_3->addButton(ui->dataYesRadio_3,1);
    dataGroup_3->addButton(ui->dataNoRadio_3,2);

    dataGroup_4 = new QButtonGroup(this);
    dataGroup_4->setExclusive(true);
    dataGroup_4->addButton(ui->dataYesRadio_4,1);
    dataGroup_4->addButton(ui->dataNoRadio_4,2);

    dataGroup_5 = new QButtonGroup(this);
    dataGroup_5->setExclusive(true);
    dataGroup_5->addButton(ui->dataYesRadio_5,1);
    dataGroup_5->addButton(ui->dataNoRadio_5,2);

    dataGroup_6 = new QButtonGroup(this);
    dataGroup_6->setExclusive(true);
    dataGroup_6->addButton(ui->dataYesRadio_6,1);
    dataGroup_6->addButton(ui->dataNoRadio_6,2);

    dataGroup_7 = new QButtonGroup(this);
    dataGroup_7->setExclusive(true);
    dataGroup_7->addButton(ui->dataYesRadio_7,1);
    dataGroup_7->addButton(ui->dataNoRadio_7,2);

    dataGroup_8 = new QButtonGroup(this);
    dataGroup_8->setExclusive(true);
    dataGroup_8->addButton(ui->dataYesRadio_8,1);
    dataGroup_8->addButton(ui->dataNoRadio_8,2);
}

void editSecDialog::setScrollBar()
{
    ui->scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                         "{"
                                                         "width:8px;"
                                                         "background:rgba(0,0,0,0%);"
                                                         "margin:0px,0px,0px,0px;"
                                                         "padding-top:9px;"
                                                         "padding-bottom:9px;"
                                                         "}"
                                                         "QScrollBar::handle:vertical"
                                                         "{"
                                                         "width:8px;"
                                                         "background:rgba(0,0,0,25%);"
                                                         " border-radius:4px;"
                                                         "min-height:20;"
                                                         "}"
                                                         "QScrollBar::handle:vertical:hover"
                                                         "{"
                                                         "width:8px;"
                                                         "background:rgba(0,0,0,50%);"
                                                         " border-radius:4px;"
                                                         "min-height:20;"
                                                         "}"
                                                         "QScrollBar::add-line:vertical"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/3.png);"
                                                         "subcontrol-position:bottom;"
                                                         "}"
                                                         "QScrollBar::sub-line:vertical"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/1.png);"
                                                         "subcontrol-position:top;"
                                                         "}"
                                                         "QScrollBar::add-line:vertical:hover"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/4.png);"
                                                         "subcontrol-position:bottom;"
                                                         "}"
                                                         "QScrollBar::sub-line:vertical:hover"
                                                         "{"
                                                         "height:9px;width:8px;"
                                                         "border-image:url(:/images/image/2.png);"
                                                         "subcontrol-position:top;"
                                                         "}"
                                                         "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                         "{"
                                                         "background:rgba(0,0,0,10%);"
                                                         "border-radius:4px;"
                                                         "}");
}
