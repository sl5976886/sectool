#ifndef CMYIPADDREDIT_H
#define CMYIPADDREDIT_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include "cmyippartlineedit.h"

class CMyIpAddrEdit : public QWidget
{
    Q_OBJECT

public:
    CMyIpAddrEdit(QWidget *parent = 0);
    ~CMyIpAddrEdit();

    void setText(const QString &text);
    QString text();
    void setStyleSheet(const QString &styleSheet);
    void setEnabled(bool);
signals:
    void textchanged(const QString& text);
    void textedited(const QString &text);

public slots:
    void onKeyPressLeftOrRight(int,int);
protected:
    void resizeEvent(QResizeEvent* event);

private slots:
    void textchangedslot(const QString& text);
    void texteditedslot(const QString &text);

private:
    CMyIpPartLineEdit *ip_part1_;
    CMyIpPartLineEdit *ip_part2_;
    CMyIpPartLineEdit *ip_part3_;
    CMyIpPartLineEdit *ip_part4_;

    QLabel *labeldot1_;
    QLabel *labeldot2_;
    QLabel *labeldot3_;
};

#endif // CMYIPADDREDIT_H
