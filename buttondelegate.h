#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QItemDelegate>
#include "renderthread.h"

class ButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ButtonDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:

public slots:

private:
    QMap<QModelIndex, QStyleOptionButton*> m_btns;
    RenderThread render;

};

#endif // BUTTONDELEGATE_H
