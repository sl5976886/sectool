#ifndef DISPOSEDIALOG2_H
#define DISPOSEDIALOG2_H

#include <QDialog>
#include <QStandardItemModel>
#include "tablemodel.h"
#include "buttondelegate.h"

namespace Ui {
class disposedialog2;
}

class disposedialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit disposedialog2(QWidget *parent = 0);
    ~disposedialog2();
    void tableViewInit();

private:
    Ui::disposedialog2 *ui;
    TableModel *m_model;
    QStandardItemModel *emptyModel;
    ButtonDelegate *m_buttonDelegate;
};

#endif // DISPOSEDIALOG2_H
