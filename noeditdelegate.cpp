#include "noeditdelegate.h"

NoEditDelegate::NoEditDelegate(QObject *parent)
{
    Q_UNUSED(parent);
}

NoEditDelegate::~NoEditDelegate()
{

}


QWidget *NoEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return NULL;
}
