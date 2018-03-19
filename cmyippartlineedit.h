#ifndef CMYIPPARTLINEEDIT_H
#define CMYIPPARTLINEEDIT_H

#include <QLineEdit>

class CMyIpPartLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    CMyIpPartLineEdit(QWidget *parent = 0,int nType = 0);
    ~CMyIpPartLineEdit();

    void set_nexttab_edit(QLineEdit *nexttab) { next_tab_ = nexttab; }
    void setEditCursorPosition(int nType);
public:signals:
    void keyPressLeftOrRight(int,int);

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    void text_edited(const QString& text);

private:
    QLineEdit *next_tab_;
    int m_nType;
};
#endif // CMYIPPARTLINEEDIT_H
