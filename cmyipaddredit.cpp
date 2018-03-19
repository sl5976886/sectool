#include "cmyipaddredit.h"
#include <QValidator>
#include <QMessageBox>

CMyIpAddrEdit::CMyIpAddrEdit(QWidget *parent)
    : QWidget(parent)
{
    ip_part1_ = new CMyIpPartLineEdit(this,0);
    ip_part2_ = new CMyIpPartLineEdit(this,1);
    ip_part3_ = new CMyIpPartLineEdit(this,2);
    ip_part4_ = new CMyIpPartLineEdit(this,3);

    labeldot1_ = new QLabel(this);
    labeldot2_ = new QLabel(this);
    labeldot3_ = new QLabel(this);

    QFont ft;
    ft.setPointSize(12);
    labeldot1_->setText(" .");
    labeldot1_->setAlignment(Qt::AlignCenter);
    labeldot1_->setFont(ft);

    labeldot2_->setText(" .");
    labeldot2_->setAlignment(Qt::AlignCenter);
    labeldot2_->setFont(ft);

    labeldot3_->setText(" .");
    labeldot3_->setAlignment(Qt::AlignCenter);
    labeldot3_->setFont(ft);

    QWidget::setTabOrder(ip_part1_, ip_part2_);
    QWidget::setTabOrder(ip_part2_, ip_part3_);
    QWidget::setTabOrder(ip_part3_, ip_part4_);
    ip_part1_->set_nexttab_edit(ip_part2_);
    ip_part2_->set_nexttab_edit(ip_part3_);
    ip_part3_->set_nexttab_edit(ip_part4_);


    connect(ip_part1_, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));
    connect(ip_part2_, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));
    connect(ip_part3_, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));
    connect(ip_part4_, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));

    connect(ip_part1_, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));
    connect(ip_part2_, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));
    connect(ip_part3_, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));
    connect(ip_part4_, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));

    connect(ip_part1_, SIGNAL(keyPressLeftOrRight(int,int)), this, SLOT(onKeyPressLeftOrRight(int,int)));
    connect(ip_part2_, SIGNAL(keyPressLeftOrRight(int,int)), this, SLOT(onKeyPressLeftOrRight(int,int)));
    connect(ip_part3_, SIGNAL(keyPressLeftOrRight(int,int)), this, SLOT(onKeyPressLeftOrRight(int,int)));
    connect(ip_part4_, SIGNAL(keyPressLeftOrRight(int,int)), this, SLOT(onKeyPressLeftOrRight(int,int)));
}

CMyIpAddrEdit::~CMyIpAddrEdit()
{

}

void CMyIpAddrEdit::textchangedslot(const QString& /*text*/)
{
    QString ippart1, ippart2, ippart3, ippart4;
    ippart1 = ip_part1_->text();
    ippart2 = ip_part2_->text();
    ippart3 = ip_part3_->text();
    ippart4 = ip_part4_->text();

    QString ipaddr = QString("%1.%2.%3.%4")
        .arg(ippart1)
        .arg(ippart2)
        .arg(ippart3)
        .arg(ippart4);

    emit textchanged(ipaddr);
}

void CMyIpAddrEdit::texteditedslot(const QString &/*text*/)
{
    QString ippart1, ippart2, ippart3, ippart4;
    ippart1 = ip_part1_->text();
    ippart2 = ip_part2_->text();
    ippart3 = ip_part3_->text();
    ippart4 = ip_part4_->text();

    QString ipaddr = QString("%1.%2.%3.%4")
        .arg(ippart1)
        .arg(ippart2)
        .arg(ippart3)
        .arg(ippart4);

    emit textedited(ipaddr);
}

void CMyIpAddrEdit::setText(const QString &text)
{
    QString ippart1, ippart2, ippart3, ippart4;
    QString qstring_validate = text;

    // IP地址验证
    QRegExp regexp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator regexp_validator(regexp, this);
    int nPos = 0;
    QValidator::State state = regexp_validator.validate(qstring_validate, nPos);
    // IP合法
    if (state == QValidator::Acceptable)
    {
        QStringList ippartlist = text.split(".");

        int strcount = ippartlist.size();
        int index = 0;
        if (index < strcount)
        {
            ippart1 = ippartlist.at(index);
        }
        if (++index < strcount)
        {
            ippart2 = ippartlist.at(index);
        }
        if (++index < strcount)
        {
            ippart3 = ippartlist.at(index);
        }
        if (++index < strcount)
        {
            ippart4 = ippartlist.at(index);
        }
    }
    ip_part1_->setText(ippart1);
    ip_part2_->setText(ippart2);
    ip_part3_->setText(ippart3);
    ip_part4_->setText(ippart4);
}

QString CMyIpAddrEdit::text()
{
    QString ippart1, ippart2, ippart3, ippart4;
    ippart1 = ip_part1_->text();
    ippart2 = ip_part2_->text();
    ippart3 = ip_part3_->text();
    ippart4 = ip_part4_->text();

    return QString("%1.%2.%3.%4")
        .arg(ippart1)
        .arg(ippart2)
        .arg(ippart3)
        .arg(ippart4);
}

void CMyIpAddrEdit::setStyleSheet(const QString &styleSheet)
{
    ip_part1_->setStyleSheet(styleSheet);
    ip_part2_->setStyleSheet(styleSheet);
    ip_part3_->setStyleSheet(styleSheet);
    ip_part4_->setStyleSheet(styleSheet);
}

void CMyIpAddrEdit::onKeyPressLeftOrRight(int nkeyType,int nNewLineEditType)
{
    switch(nNewLineEditType)
    {
    case 0:
        ip_part1_->setEditCursorPosition(nkeyType);
        break;
    case 1:
        ip_part2_->setEditCursorPosition(nkeyType);
        break;
    case 2:
        ip_part3_->setEditCursorPosition(nkeyType);
        break;
    case 3:
        ip_part4_->setEditCursorPosition(nkeyType);
        break;
    }
}

void CMyIpAddrEdit::setEnabled(bool bEnable)
{
    ip_part1_->setEnabled(bEnable);
    ip_part2_->setEnabled(bEnable);
    ip_part3_->setEnabled(bEnable);
    ip_part4_->setEnabled(bEnable);

    labeldot1_->setEnabled(bEnable);
    labeldot2_->setEnabled(bEnable);
    labeldot3_->setEnabled(bEnable);
}

void CMyIpAddrEdit::resizeEvent(QResizeEvent* event)
{
    int nWidth = width()/4;
    int nHeight = height();
    ip_part1_->setGeometry(QRect(0, 0, nWidth, nHeight));
    ip_part2_->setGeometry(QRect(nWidth, 0, nWidth, nHeight));
    ip_part3_->setGeometry(QRect(nWidth*2, 0, nWidth, nHeight));
    ip_part4_->setGeometry(QRect(nWidth*3, 0, width()-nWidth*3, nHeight));

    labeldot1_->setGeometry(QRect(nWidth-3, 1, 6, nHeight-2));
    labeldot2_->setGeometry(QRect(nWidth*2-3, 1, 6, nHeight-2));
    labeldot3_->setGeometry(QRect(nWidth*3-3, 1, 6, nHeight-2));
}
