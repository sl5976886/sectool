#ifndef NOEDITDELEGATE_H
#define NOEDITDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>

class NoEditDelegate:public QStyledItemDelegate
{
public:
    NoEditDelegate(QObject *parent=0);
    ~NoEditDelegate();
    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
