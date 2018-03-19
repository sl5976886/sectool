#include "editwebdialog.h"
#include "ui_editwebdialog.h"
#include "commonfunction.h"
#include <QScrollBar>
#include <QLayout>
#include <QRadioButton>

editWebDialog::editWebDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editWebDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle(QString("编辑记录单"));
    //setScrollView();
    certainButton = new QPushButton(QString("确定"),this);
    QHBoxLayout *mLayout = new QHBoxLayout(this);
    mLayout->addWidget(certainButton);
    mLayout->setMargin(0);
    ui->buttonLabel->setLayout(mLayout);
    QObject::connect(certainButton,SIGNAL(clicked()),this,SLOT(updateDate()));

    initRadio();
    initDataShow();
}

editWebDialog::~editWebDialog()
{
    delete ui;
}

void editWebDialog::initDataShow()
{
    QString str_1 = webList[0];
    ui->lineEdit->setText(str_1);

    QString str_2 = webList[1];
    ui->lineEdit_2->setText(str_2);

    QString str_3 = webList[2];
    ui->lineEdit_3->setText(str_3);

    QString str_4 = webList[3];
    ui->lineEdit_4->setText(str_4);

    QString str_5 = webList[4];
    ui->lineEdit_5->setText(str_5);

    QString str_6 = webList[5];
    ui->lineEdit_6->setText(str_6);

    QString str_7 = webList[6];
    ui->lineEdit_7->setText(str_7);

    QString str_8 = webList[7];
    ui->lineEdit_8->setText(str_8);

    QString str_9 = webList[8];
    ui->lineEdit_9->setText(str_9);

    QString str_10 = webList[9];
    ui->lineEdit_10->setText(str_10);

    QString str_11 = webList[10];
    ui->lineEdit_11->setText(str_11);

    QString str_12 = webList[11];
    ui->lineEdit_12->setText(str_12);

    QString str_13 = webList[12];
    if(str_13==QString("地(区、市、州、盟)"))
    {
        ui->relationRadio_1->setChecked(true);
    }
    else if(str_13==QString("省(自治区、直辖市)"))
    {
        ui->relationRadio_2->setChecked(true);
    }
    else if(str_13==QString("中央"))
    {
        ui->relationRadio_3->setChecked(true);
    }
    else if(str_13==QString("县(区、市、旗)"))
    {
        ui->relationRadio_4->setChecked(true);
    }
    else if(str_13==QString("其他"))
    {
        ui->relationRadio_5->setChecked(true);
    }

    QString str_14 = webList[13];
    if(str_14==QString("事业单位"))
    {
        ui->comRadio_1->setChecked(true);
    }
    else if(str_14==QString("政府机关"))
    {
        ui->comRadio_2->setChecked(true);
    }
    else if(str_14==QString("党委机关"))
    {
        ui->comRadio_3->setChecked(true);
    }
    else if(str_14==QString("企业"))
    {
        ui->comRadio_4->setChecked(true);
    }
    else if(str_14==QString("其他"))
    {
        ui->comRadio_5->setChecked(true);
    }

    QString str_15 = webList[14];
    if(str_15==QString("电信"))
    {
        ui->induRadio_1->setChecked(true);
    }
    else if(str_15==QString("广电"))
    {
        ui->induRadio_2->setChecked(true);
    }
    else if(str_15==QString("经营性公众互联网"))
    {
        ui->induRadio_3->setChecked(true);
    }
    else if(str_15==QString("铁路"))
    {
        ui->induRadio_4->setChecked(true);
    }
    else if(str_15==QString("银行"))
    {
        ui->induRadio_5->setChecked(true);
    }
    else if(str_15==QString("海关"))
    {
        ui->induRadio_6->setChecked(true);
    }
    else if(str_15==QString("税务"))
    {
        ui->induRadio_7->setChecked(true);
    }
    else if(str_15==QString("民航"))
    {
        ui->induRadio_8->setChecked(true);
    }
    else if(str_15==QString("电力"))
    {
        ui->induRadio_9->setChecked(true);
    }
    else if(str_15==QString("证券"))
    {
        ui->induRadio_10->setChecked(true);
    }
    else if(str_15==QString("保险"))
    {
        ui->induRadio_11->setChecked(true);
    }
    else if(str_15==QString("国防科技工业"))
    {
        ui->induRadio_12->setChecked(true);
    }
    else if(str_15==QString("公安"))
    {
        ui->induRadio_13->setChecked(true);
    }
    else if(str_15==QString("人事劳动和社会保障"))
    {
        ui->induRadio_14->setChecked(true);
    }
    else if(str_15==QString("财政"))
    {
        ui->induRadio_15->setChecked(true);
    }
    else if(str_15==QString("审计"))
    {
        ui->induRadio_16->setChecked(true);
    }
    else if(str_15==QString("商业贸易"))
    {
        ui->induRadio_17->setChecked(true);
    }
    else if(str_15==QString("国土资源"))
    {
        ui->induRadio_18->setChecked(true);
    }
    else if(str_15==QString("能源"))
    {
        ui->induRadio_19->setChecked(true);
    }
    else if(str_15==QString("交通"))
    {
        ui->induRadio_20->setChecked(true);
    }
    else if(str_15==QString("统计"))
    {
        ui->induRadio_21->setChecked(true);
    }
    else if(str_15==QString("工商行政管理"))
    {
        ui->induRadio_22->setChecked(true);
    }
    else if(str_15==QString("邮政"))
    {
        ui->induRadio_23->setChecked(true);
    }
    else if(str_15==QString("教育"))
    {
        ui->induRadio_24->setChecked(true);
    }
    else if(str_15==QString("文化"))
    {
        ui->induRadio_25->setChecked(true);
    }
    else if(str_15==QString("卫生"))
    {
        ui->induRadio_26->setChecked(true);
    }
    else if(str_15==QString("农业"))
    {
        ui->induRadio_27->setChecked(true);
    }
    else if(str_15==QString("水利"))
    {
        ui->induRadio_28->setChecked(true);
    }
    else if(str_15==QString("外交"))
    {
        ui->induRadio_29->setChecked(true);
    }
    else if(str_15==QString("发展改革"))
    {
        ui->induRadio_30->setChecked(true);
    }
    else if(str_15==QString("科技"))
    {
        ui->induRadio_31->setChecked(true);
    }
    else if(str_15==QString("宣传"))
    {
        ui->induRadio_32->setChecked(true);
    }
    else if(str_15==QString("质量监督检验检疫"))
    {
        ui->induRadio_33->setChecked(true);
    }
    else
    {
        ui->induRadio_34->setChecked(true);
    }

    QString str_16 = webList[15];
    if(str_16==QString("二级"))
    {
        ui->dbRadio_1->setChecked(true);
    }
    else if(str_16==QString("三级"))
    {
        ui->dbRadio_2->setChecked(true);
    }
    else if(str_16==QString("四级"))
    {
        ui->dbRadio_3->setChecked(true);
    }
    else if(str_16==QString("未定级"))
    {
        ui->dbRadio_4->setChecked(true);
    }

    QString str_17 = webList[16];
    if(str_17==QString("已展开"))
    {
        ui->djpcRadio_1->setChecked(true);
    }
    else if(str_17==QString("未展开"))
    {
        ui->djpcRadio_2->setChecked(true);
    }

    QString str_18 = webList[17];
    if(str_18==QString("已签订"))
    {
        ui->zrsRadio_1->setChecked(true);
    }
    else if(str_18==QString("未签订"))
    {
        ui->zrsRadio_2->setChecked(true);
    }

    QString str_19 = webList[18];
    if(str_19==QString("新闻发布"))
    {
        ui->fwlmRadio_1->setChecked(true);
    }
    if(str_19==QString("政策宣传"))
    {
        ui->fwlmRadio_2->setChecked(true);
    }
    if(str_19==QString("事项办理"))
    {
        ui->fwlmRadio_3->setChecked(true);
    }
    if(str_19==QString("论坛"))
    {
        ui->fwlmRadio_4->setChecked(true);
    }
    if(str_19==QString("即时通讯"))
    {
        ui->fwlmRadio_5->setChecked(true);
    }
    if(str_19==QString("电子邮件"))
    {
        ui->fwlmRadio_6->setChecked(true);
    }
    if(str_19==QString("留言板"))
    {
        ui->fwlmRadio_7->setChecked(true);
    }
    if(str_19==QString("政务公开"))
    {
        ui->fwlmRadio_8->setChecked(true);
    }
    if(str_19==QString("其他"))
    {
        ui->fwlmRadio_9->setChecked(true);
    }
}

void editWebDialog::updateDate()
{
    webList.clear();
    webList.append(ui->lineEdit->text());
    webList.append(ui->lineEdit_2->text());
    webList.append(ui->lineEdit_3->text());
    webList.append(ui->lineEdit_4->text());
    webList.append(ui->lineEdit_5->text());
    webList.append(ui->lineEdit_6->text());
    webList.append(ui->lineEdit_7->text());
    webList.append(ui->lineEdit_8->text());
    webList.append(ui->lineEdit_9->text());
    webList.append(ui->lineEdit_10->text());
    webList.append(ui->lineEdit_11->text());
    webList.append(ui->lineEdit_12->text());
    quint16 relation_id = relationGroup->checkedId();
    switch (relation_id)
    {
    case 1:
        webList.append(QString("地(区、市、州、盟)"));
        break;
    case 2:
        webList.append(QString("省(自治区、直辖市)"));
        break;
    case 3:
        webList.append(QString("中央"));
        break;
    case 4:
        webList.append(QString("县(区、市、旗)"));
        break;
    case 5:
        webList.append(QString("其他"));
        break;
    default:
        webList.append(QString(""));
        break;
    }

    quint16 com_id = comGroup->checkedId();
    switch (com_id)
    {
    case 1:
        webList.append(QString("事业单位"));
        break;
    case 2:
        webList.append(QString("政府机关"));
        break;
    case 3:
        webList.append(QString("党委机关"));
        break;
    case 4:
        webList.append(QString("企业"));
        break;
    case 5:
        webList.append(QString("其他"));
        break;
    default:
        webList.append(QString(""));
        break;
    }

    quint16 indu_id = induGroup->checkedId();
    switch (indu_id)
    {
    case 1:
        webList.append(QString("电信"));
        break;
    case 2:
        webList.append(QString("广电"));
        break;
    case 3:
        webList.append(QString("经营性公众互联网"));
        break;
    case 4:
        webList.append(QString("铁路"));
        break;
    case 5:
        webList.append(QString("银行"));
        break;
    case 6:
        webList.append(QString("海关"));
        break;
    case 7:
        webList.append(QString("税务"));
        break;
    case 8:
        webList.append(QString("民航"));
        break;
    case 9:
        webList.append(QString("电力"));
        break;
    case 10:
        webList.append(QString("证券"));
        break;
    case 11:
        webList.append(QString("保险"));
        break;
    case 12:
        webList.append(QString("国防科技工业"));
        break;
    case 13:
        webList.append(QString("公安"));
        break;
    case 14:
        webList.append(QString("人事劳动和社会保障"));
        break;
    case 15:
        webList.append(QString("财政"));
        break;
    case 16:
        webList.append(QString("审计"));
        break;
    case 17:
        webList.append(QString("商业贸易"));
        break;
    case 18:
        webList.append(QString("国土资源"));
        break;
    case 19:
        webList.append(QString("能源"));
        break;
    case 20:
        webList.append(QString("交通"));
        break;
    case 21:
        webList.append(QString("统计"));
        break;
    case 22:
        webList.append(QString("工商行政管理"));
        break;
    case 23:
        webList.append(QString("邮政"));
        break;
    case 24:
        webList.append(QString("教育"));
        break;
    case 25:
        webList.append(QString("文化"));
        break;
    case 26:
        webList.append(QString("卫生"));
        break;
    case 27:
        webList.append(QString("农业"));
        break;
    case 28:
        webList.append(QString("水利"));
        break;
    case 29:
        webList.append(QString("外交"));
        break;
    case 30:
        webList.append(QString("发展改革"));
        break;
    case 31:
        webList.append(QString("科技"));
        break;
    case 32:
        webList.append(QString("宣传"));
        break;
    case 33:
        webList.append(QString("质量监督检验检疫"));
        break;
    case 34:
        webList.append(ui->induLineEdit->text());
        break;
    default:
        webList.append(QString(""));
        break;
    }

    quint16 db_id = dbGroup->checkedId();
    switch (db_id)
    {
    case 1:
        webList.append(QString("二级"));
        break;
    case 2:
        webList.append(QString("三级"));
        break;
    case 3:
        webList.append(QString("四级"));
        break;
    case 4:
        webList.append(QString("未定级"));
        break;
    default:
        webList.append(QString(""));
        break;
    }

    quint16 djpc_id = djpcGroup->checkedId();
    switch (djpc_id)
    {
    case 1:
        webList.append(QString("已展开"));
        break;
    case 2:
        webList.append(QString("未展开"));
        break;
    default:
        webList.append(QString(""));
        break;
    }

    quint16 zrs_id = zrsGroup->checkedId();
    switch (zrs_id)
    {
    case 1:
        webList.append(QString("已签订"));
        break;
    case 2:
        webList.append(QString("未签订"));
        break;
    default:
        webList.append(QString(""));
        break;
    }

    quint16 fwlm_id = fwlmGroup->checkedId();
    switch (fwlm_id)
    {
    case 1:
        webList.append(QString("新闻发布"));
        break;
    case 2:
        webList.append(QString("政策宣传"));
        break;
    case 3:
        webList.append(QString("事项办理"));
        break;
    case 4:
        webList.append(QString("论坛"));
        break;
    case 5:
        webList.append(QString("即时通讯"));
        break;
    case 6:
        webList.append(QString("电子邮件"));
        break;
    case 7:
        webList.append(QString("留言板"));
        break;
    case 8:
        webList.append(QString("政务公开"));
        break;
    case 9:
        webList.append(QString("其他"));
        break;
    default:
        webList.append(QString(""));
        break;
    }

    this->close();
}

void editWebDialog::initRadio()
{
    relationGroup = new QButtonGroup(this);
    relationGroup->setExclusive(true);
    relationGroup->addButton(ui->relationRadio_1,1);
    relationGroup->addButton(ui->relationRadio_2,2);
    relationGroup->addButton(ui->relationRadio_3,3);
    relationGroup->addButton(ui->relationRadio_4,4);
    relationGroup->addButton(ui->relationRadio_5,5);
    ui->relationRadio_1->setChecked(true);

    comGroup = new QButtonGroup(this);
    comGroup->setExclusive(true);
    comGroup->addButton(ui->comRadio_1,1);
    comGroup->addButton(ui->comRadio_2,2);
    comGroup->addButton(ui->comRadio_3,3);
    comGroup->addButton(ui->comRadio_4,4);
    comGroup->addButton(ui->comRadio_5,5);
    ui->comRadio_1->setChecked(true);

    induGroup = new QButtonGroup(this);
    induGroup->setExclusive(true);
    induGroup->addButton(ui->induRadio_1,1);
    induGroup->addButton(ui->induRadio_2,2);
    induGroup->addButton(ui->induRadio_3,3);
    induGroup->addButton(ui->induRadio_4,4);
    induGroup->addButton(ui->induRadio_5,5);
    induGroup->addButton(ui->induRadio_6,6);
    induGroup->addButton(ui->induRadio_7,7);
    induGroup->addButton(ui->induRadio_8,8);
    induGroup->addButton(ui->induRadio_9,9);
    induGroup->addButton(ui->induRadio_10,10);
    induGroup->addButton(ui->induRadio_11,11);
    induGroup->addButton(ui->induRadio_12,12);
    induGroup->addButton(ui->induRadio_13,13);
    induGroup->addButton(ui->induRadio_14,14);
    induGroup->addButton(ui->induRadio_15,15);
    induGroup->addButton(ui->induRadio_16,16);
    induGroup->addButton(ui->induRadio_17,17);
    induGroup->addButton(ui->induRadio_18,18);
    induGroup->addButton(ui->induRadio_19,19);
    induGroup->addButton(ui->induRadio_20,20);
    induGroup->addButton(ui->induRadio_21,21);
    induGroup->addButton(ui->induRadio_22,22);
    induGroup->addButton(ui->induRadio_23,23);
    induGroup->addButton(ui->induRadio_24,24);
    induGroup->addButton(ui->induRadio_25,25);
    induGroup->addButton(ui->induRadio_26,26);
    induGroup->addButton(ui->induRadio_27,27);
    induGroup->addButton(ui->induRadio_28,28);
    induGroup->addButton(ui->induRadio_29,29);
    induGroup->addButton(ui->induRadio_30,30);
    induGroup->addButton(ui->induRadio_31,31);
    induGroup->addButton(ui->induRadio_32,32);
    induGroup->addButton(ui->induRadio_33,33);
    induGroup->addButton(ui->induRadio_34,34);
    ui->induRadio_1->setChecked(true);

    dbGroup = new QButtonGroup(this);
    dbGroup->setExclusive(true);
    dbGroup->addButton(ui->dbRadio_1,1);
    dbGroup->addButton(ui->dbRadio_2,2);
    dbGroup->addButton(ui->dbRadio_3,3);
    dbGroup->addButton(ui->dbRadio_4,4);
    ui->dbRadio_1->setChecked(true);

    djpcGroup = new QButtonGroup(this);
    djpcGroup->setExclusive(true);
    djpcGroup->addButton(ui->djpcRadio_1,1);
    djpcGroup->addButton(ui->djpcRadio_2,2);
    ui->djpcRadio_1->setChecked(true);

    zrsGroup = new QButtonGroup(this);
    zrsGroup->setExclusive(true);
    zrsGroup->addButton(ui->zrsRadio_1,1);
    zrsGroup->addButton(ui->zrsRadio_2,2);
    ui->zrsRadio_1->setChecked(true);

    fwlmGroup = new QButtonGroup(this);
    fwlmGroup->setExclusive(true);
    fwlmGroup->addButton(ui->fwlmRadio_1,1);
    fwlmGroup->addButton(ui->fwlmRadio_2,2);
    fwlmGroup->addButton(ui->fwlmRadio_3,3);
    fwlmGroup->addButton(ui->fwlmRadio_4,4);
    fwlmGroup->addButton(ui->fwlmRadio_5,5);
    fwlmGroup->addButton(ui->fwlmRadio_6,6);
    fwlmGroup->addButton(ui->fwlmRadio_7,7);
    fwlmGroup->addButton(ui->fwlmRadio_8,8);
    fwlmGroup->addButton(ui->fwlmRadio_9,9);
    ui->fwlmRadio_1->setChecked(true);
}

void editWebDialog::setScrollView()
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
