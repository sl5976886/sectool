#include "CMyIpPartLineEdit.h"
#include <QIntValidator>
#include <QKeyEvent>

CMyIpPartLineEdit::CMyIpPartLineEdit(QWidget *parent/* = 0*/,int nType)
: QLineEdit(parent)
{
    next_tab_ = NULL;

    this->setMaxLength(3);
    this->setFrame(false);
    this->setAlignment(Qt::AlignCenter);

    QValidator *validator = new QIntValidator(0, 255, this);
    this->setValidator(validator);
    switch(nType)
    {
    case 0:
        //第一个
        setStyleSheet("QLineEdit{border-right:0px;}");
        break;
    case 1:
        //第二个
        setStyleSheet("QLineEdit{border-left:0px;border-right:0px;}");
        break;
    case 2:
        //第三个
        setStyleSheet("QLineEdit{border-left:0px;border-right:0px;}");
        break;
    case 3:
        setStyleSheet("QLineEdit{border-left:0px;}");
        break;
    }
    m_nType = nType;

    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(text_edited(const QString&)));
}

CMyIpPartLineEdit::~CMyIpPartLineEdit(void)
{
}

void CMyIpPartLineEdit::focusInEvent(QFocusEvent *e)
{
    this->selectAll();
    QLineEdit::focusInEvent(e);
}

void CMyIpPartLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Period)
    {
        if (next_tab_)
        {
            next_tab_->setFocus();
            next_tab_->selectAll();
        }
    }
    else if (event->key() == Qt::Key_Left)
    {
        if (cursorPosition() == 0&&m_nType!=0)
        {
            emit keyPressLeftOrRight((int)Qt::Key_Left,m_nType-1);
        }
    }
    else if (event->key() == Qt::Key_Right)
    {
        if (cursorPosition() == text().size()&&m_nType!=3)
        {
            emit keyPressLeftOrRight((int)Qt::Key_Right,m_nType+1);
        }
    }
    QLineEdit::keyPressEvent(event);
}

void CMyIpPartLineEdit::text_edited(const QString& text)
{
    QIntValidator v(0, 255, this);
    QString ipaddr = text;
    int pos = 0;
    QValidator::State state = v.validate(ipaddr, pos);
    if (state == QValidator::Acceptable)
    {
        if (ipaddr.size() > 1)
        {
            if (ipaddr.size() == 2)
            {
                int ipnum = ipaddr.toInt();

                if (ipnum > 25)
                {
                    if (next_tab_)
                    {
                        next_tab_->setFocus();
                        next_tab_->selectAll();
                    }
                }
            }
            else
            {
                if (next_tab_)
                {
                    next_tab_->setFocus();
                    next_tab_->selectAll();
                }
            }
        }
    }
}


void CMyIpPartLineEdit::setEditCursorPosition(int nType)
{
    setFocus();
    int nSize = 0;
    if (nType == Qt::Key_Left)
    {
        nSize = text().size();
    }
    setCursorPosition(nSize);
}
