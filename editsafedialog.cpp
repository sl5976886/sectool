#include "editsafedialog.h"
#include "ui_editsafedialog.h"
#include "commonfunction.h"
#include <QScrollBar>
#include <QLayout>
#include <QRadioButton>

editSafeDialog::editSafeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editSafeDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(QString("编辑记录单"));

    certainButton = new QPushButton(QString("确定"),this);
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);
    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(certain_click()));

    //setScrollView();
    initRadio();
}

editSafeDialog::~editSafeDialog()
{
    delete ui;
}

void editSafeDialog::certain_click()
{
    safeList.clear();
    QString str;
    if(ui->attackRadio_2->isChecked())
    {
        str = QString("没有部署网站防攻击设备；");
    }
    else if(ui->attackRadio_1->isChecked())
    {
        str = QString("部署了网站防攻击设备：");
        if(ui->antiCheckBox_1->isChecked())
        {
            str+=QString("其他,型号：");
            str+=ui->antiLineEdit_1->text();
            str+=QString("；");
        }
        if(ui->antiCheckBox_2->isChecked())
        {
            str+=QString("防火墙,型号：");
            str+=ui->antiLineEdit_2->text();
            str+=QString("；");
        }
        if(ui->antiCheckBox_3->isChecked())
        {
            str+=QString("IPS,型号：");
            str+=ui->antiLineEdit_3->text();
            str+=QString("；");
        }
        if(ui->antiCheckBox_4->isChecked())
        {
            str+=QString("UTM,型号：");
            str+=ui->antiLineEdit_4->text();
            str+=QString("；");
        }
        if(ui->antiCheckBox_5->isChecked())
        {
            str+=QString("WAF,型号：");
            str+=ui->antiLineEdit_5->text();
            str+=QString("；");
        }
        if(ui->passRadio_1->isChecked())
        {
            str+=QString("采用默认管理口令");
        }
        else if(ui->passRadio_2->isChecked())
        {
            str+=QString("没有采用默认管理口令");
        }
    }
    else
    {
        str = QString("");
    }
    safeList.append(str);

    str = QString("");
    if(ui->tamperRadio_1->isChecked())
    {
        str+=QString("部署了应用程序防篡改措施；");
    }
    else if(ui->tamperRadio_2->isChecked())
    {
        str+=QString("没有部署应用程序防篡改措施；");
    }
    if(ui->hostRadio_1->isChecked())
    {
        str+=QString("部署了主机加固措施；");
    }
    else if(ui->hostRadio_2->isChecked())
    {
        str+=QString("没有部署主机加固措施；");
    }
    safeList.append(str);

    str = QString("");
    if(ui->antiRadio_1->isChecked())
    {
        str+=QString("部署了防病毒网关系统；");
    }
    else if(ui->antiRadio_2->isChecked())
    {
        str+=QString("没有部署防病毒网关系统；");
    }
    if(ui->serverRadio_1->isChecked())
    {
        str+=QString("部署了服务器版防病毒系统；");
    }
    else if(ui->serverRadio_2->isChecked())
    {
        str+=QString("没有部署服务器版防病毒系统；");
    }
    if(ui->updateRadio_1->isChecked())
    {
        str+=QString("病毒库自动更新；");
    }
    else if(ui->updateRadio_2->isChecked())
    {
        str+=QString("病毒库定期更新；");
    }
    if(ui->existRadio_1->isChecked())
    {
        str+=QString("发现病毒；");
    }
    else if(ui->existRadio_2->isChecked())
    {
        str+=QString("没有发现病毒；");
    }
    safeList.append(str);

    str = QString("");
    if(ui->beifenRadio_1->isChecked())
    {
        str+=QString("对网站服务器进行了备份；");
    }
    else if(ui->beifenRadio_2->isChecked())
    {
        str+=QString("没有对网站服务器进行了备份；");
    }
    if(ui->fangshiRadio_1->isChecked())
    {
        str+=QString("备份方式为双机热备；");
    }
    else if(ui->fangshiRadio_2->isChecked())
    {
        str+=QString("备份方式为冷备；");
    }
    else if(ui->fangshiRadio_3->isChecked())
    {
        str+=QString("备份方式为");
        str+=ui->fangshiLineEdit->text();
        str+=QString("；");
    }
    safeList.append(str);

    str = QString("");
    if(ui->shenjiRadio_1->isChecked())
    {
        str+=QString("部署了审计系统；");
    }
    else if(ui->shenjiRadio_2->isChecked())
    {
        str+=QString("没有部署审计系统；");
    }
    if(ui->aqxgRadio_1->isChecked())
    {
        str+=QString("对重要的安全相关事件进行审计；");
    }
    else if(ui->aqxgRadio_2->isChecked())
    {
        str+=QString("没有对重要的安全相关事件进行审计；");
    }
    if(ui->sjbbRadio_1->isChecked())
    {
        str+=QString("生成了审计报表；");
    }
    else if(ui->sjbbRadio_2->isChecked())
    {
        str+=QString("没有生成审计报表；");
    }
    if(ui->sjbhRadio_1->isChecked())
    {
        str+=QString("对审计记录进行了保护；");
    }
    else if(ui->sjbhRadio_2->isChecked())
    {
        str+=QString("没有对审计记录进行保护；");
    }
    safeList.append(str);

    str = QString("");
    if(ui->dwfwRadio_1->isChecked())
    {
        str+=QString("部署了对外服务的应用；");
    }
    else if(ui->dwfwRadio_2->isChecked())
    {
        str+=QString("没有部署对外服务的应用；");
    }
    if(ui->aqfhRadio_1->isChecked())
    {
        str+=QString("对应用进行了安全防护；");
    }
    else if(ui->aqfhRadio_2->isChecked())
    {
        str+=QString("没有对应用进行安全防护；");
    }
    safeList.append(str);

    str = QString("");
    if(ui->dsfcjRadio_1->isChecked())
    {
        str+=QString("启用了第三方插件；");
    }
    else if(ui->dsfcjRadio_2->isChecked())
    {
        str+=QString("没有启用第三方插件；");
    }
    if(ui->dsfldRadio_1->isChecked())
    {
        str+=QString("存在第三方插件漏洞；");
    }
    else if(ui->dsfldRadio_2->isChecked())
    {
        str+=QString("不存在第三方插件漏洞；");
    }
    safeList.append(str);

    str = QString("");
    if(ui->htglRadio_1->isChecked())
    {
        str+=QString("启用了后台管理；");
    }
    else if(ui->htglRadio_2->isChecked())
    {
        str+=QString("没有启用后台管理；");
    }
    if(ui->ycljRadio_1->isChecked())
    {
        str+=QString("隐藏了后台管理路径；");
    }
    else if(ui->ycljRadio_2->isChecked())
    {
        str+=QString("没有隐藏后台管理路径；");
    }
    if(ui->mrklRadio_1->isChecked())
    {
        str+=QString("是默认口令/弱口令；");
    }
    else if(ui->mrklRadio_2->isChecked())
    {
        str+=QString("不是默认口令/弱口令；");
    }
    safeList.append(str);

    str = QString("");
    if(ui->bdgxRadio_1->isChecked())
    {
        str+=QString("进行了安全补丁更新；");
    }
    else if(ui->bdgxRadio_2->isChecked())
    {
        str+=QString("没有进行安全补丁更新；");
    }
    if(ui->bdgxplRadio_1->isChecked())
    {
        str+=QString("安全补丁自动升级；");
    }
    else if(ui->bdgxplRadio_2->isChecked())
    {
        str+=QString("安全补丁定期升级；");
    }
    if(ui->stcsRadio_1->isChecked())
    {
        str+=QString("进行了渗透测试；");
    }
    else if(ui->stcsRadio_2->isChecked())
    {
        str+=QString("没有进行渗透测试；");
    }
    if(ui->jxplRadio_1->isChecked())
    {
        str+=QString("高频率渗透测试；");
    }
    else if(ui->jxplRadio_2->isChecked())
    {
        str+=QString("低频率渗透测试；");
    }
    safeList.append(str);

    str = QString("");
    str+=QString("网站接入的宽带为：");
    str+=ui->wzfhLineEdit->text();
    str+=";";
    if(ui->cdnRadio_1->isChecked())
    {
        str+=QString("有CDN加速；");
    }
    else if(ui->cdnRadio_2->isChecked())
    {
        str+=QString("没有CDN加速；");
    }
    safeList.append(str);
    this->close();
}

void editSafeDialog::setScrollView()
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

void editSafeDialog::initRadio()
{
    attackGroup = new QButtonGroup(this);
    attackGroup->setExclusive(true);
    attackGroup->addButton(ui->attackRadio_1,1);
    attackGroup->addButton(ui->attackRadio_2,2);

    passGroup = new QButtonGroup(this);
    passGroup->setExclusive(true);
    passGroup->addButton(ui->passRadio_1,1);
    passGroup->addButton(ui->passRadio_2,2);

    tamperGroup = new QButtonGroup(this);
    tamperGroup->setExclusive(true);
    tamperGroup->addButton(ui->tamperRadio_1,1);
    tamperGroup->addButton(ui->tamperRadio_2,2);

    hostGroup = new QButtonGroup(this);
    hostGroup->setExclusive(true);
    hostGroup->addButton(ui->hostRadio_1,1);
    hostGroup->addButton(ui->hostRadio_2,2);

    antiGroup = new QButtonGroup(this);
    antiGroup->setExclusive(true);
    antiGroup->addButton(ui->antiRadio_1,1);
    antiGroup->addButton(ui->antiRadio_2,2);

    serverGroup = new QButtonGroup(this);
    serverGroup->setExclusive(true);
    serverGroup->addButton(ui->serverRadio_1,1);
    serverGroup->addButton(ui->serverRadio_2,2);

    updateGroup = new QButtonGroup(this);
    updateGroup->setExclusive(true);
    updateGroup->addButton(ui->updateRadio_1,1);
    updateGroup->addButton(ui->updateRadio_2,2);

    existGroup = new QButtonGroup(this);
    existGroup->setExclusive(true);
    existGroup->addButton(ui->existRadio_1,1);
    existGroup->addButton(ui->existRadio_2,2);

    beifenGroup = new QButtonGroup(this);
    beifenGroup->setExclusive(true);
    beifenGroup->addButton(ui->beifenRadio_1,1);
    beifenGroup->addButton(ui->beifenRadio_2,2);

    fangshiGroup = new QButtonGroup(this);
    fangshiGroup->setExclusive(true);
    fangshiGroup->addButton(ui->fangshiRadio_1,1);
    fangshiGroup->addButton(ui->fangshiRadio_2,2);
    fangshiGroup->addButton(ui->fangshiRadio_3,3);

    shenjiGroup = new QButtonGroup(this);
    shenjiGroup->setExclusive(true);
    shenjiGroup->addButton(ui->shenjiRadio_1,1);
    shenjiGroup->addButton(ui->shenjiRadio_2,2);

    aqxgGroup = new QButtonGroup(this);
    aqxgGroup->setExclusive(true);
    aqxgGroup->addButton(ui->aqxgRadio_1,1);
    aqxgGroup->addButton(ui->aqxgRadio_2,2);

    sjbbGroup = new QButtonGroup(this);
    sjbbGroup->setExclusive(true);
    sjbbGroup->addButton(ui->sjbbRadio_1,1);
    sjbbGroup->addButton(ui->sjbbRadio_2,2);

    sjbhGroup = new QButtonGroup(this);
    sjbhGroup->setExclusive(true);
    sjbhGroup->addButton(ui->sjbhRadio_1,1);
    sjbhGroup->addButton(ui->sjbhRadio_2,2);

    dwfwGroup = new QButtonGroup(this);
    dwfwGroup->setExclusive(true);
    dwfwGroup->addButton(ui->dwfwRadio_1,1);
    dwfwGroup->addButton(ui->dwfwRadio_2,2);

    aqfhGroup = new QButtonGroup(this);
    aqfhGroup->setExclusive(true);
    aqfhGroup->addButton(ui->aqfhRadio_1,1);
    aqfhGroup->addButton(ui->aqfhRadio_2,2);

    dsfcjGroup = new QButtonGroup(this);
    dsfcjGroup->setExclusive(true);
    dsfcjGroup->addButton(ui->dsfcjRadio_1,1);
    dsfcjGroup->addButton(ui->dsfcjRadio_2,2);

    dsfldGroup = new QButtonGroup(this);
    dsfldGroup->setExclusive(true);
    dsfldGroup->addButton(ui->dsfldRadio_1,1);
    dsfldGroup->addButton(ui->dsfldRadio_2,2);

    htglGroup = new QButtonGroup(this);
    htglGroup->setExclusive(true);
    htglGroup->addButton(ui->htglRadio_1,1);
    htglGroup->addButton(ui->htglRadio_2,2);

    ycljGroup = new QButtonGroup(this);
    ycljGroup->setExclusive(true);
    ycljGroup->addButton(ui->ycljRadio_1,1);
    ycljGroup->addButton(ui->ycljRadio_2,2);

    mrklGroup = new QButtonGroup(this);
    mrklGroup->setExclusive(true);
    mrklGroup->addButton(ui->mrklRadio_1,1);
    mrklGroup->addButton(ui->mrklRadio_2,2);

    bdgxGroup = new QButtonGroup(this);
    bdgxGroup->setExclusive(true);
    bdgxGroup->addButton(ui->bdgxRadio_1,1);
    bdgxGroup->addButton(ui->bdgxRadio_2,2);

    bdgxplGroup = new QButtonGroup(this);
    bdgxplGroup->setExclusive(true);
    bdgxplGroup->addButton(ui->bdgxplRadio_1,1);
    bdgxplGroup->addButton(ui->bdgxplRadio_2,2);

    stcsGroup = new QButtonGroup(this);
    stcsGroup->setExclusive(true);
    stcsGroup->addButton(ui->stcsRadio_1,1);
    stcsGroup->addButton(ui->stcsRadio_2,2);

    jxplGroup = new QButtonGroup(this);
    jxplGroup->setExclusive(true);
    jxplGroup->addButton(ui->jxplRadio_1,1);
    jxplGroup->addButton(ui->jxplRadio_2,2);

    cdnGroup = new QButtonGroup(this);
    cdnGroup->setExclusive(true);
    cdnGroup->addButton(ui->cdnRadio_1,1);
    cdnGroup->addButton(ui->cdnRadio_2,2);
}






