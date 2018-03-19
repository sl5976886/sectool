#include "disposedialog2.h"
#include "ui_disposedialog2.h"
#include "commonfunction.h"

disposedialog2::disposedialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disposedialog2)
{
    ui->setupUi(this);
    tableViewInit();
}

disposedialog2::~disposedialog2()
{
    delete ui;
    if(m_model)
    {
        m_model = NULL;
        delete m_model;
    }
    if(emptyModel)
    {
        emptyModel = NULL;
        delete emptyModel;
    }
}

void disposedialog2::tableViewInit()
{
    QSqlQuery query1;
    QString strSql = QString("SELECT * FROM check_task WHERE id = %1").arg(g_task_id);
    bool ok = query1.exec(strSql);
    if(!ok)
    {
        qWarning()<<"sql error:"<<query1.lastError();
    }
    int attcount=query1.size();
    if(attcount == 0)
    {
        emptyModel = new QStandardItemModel();
        emptyModel->setHorizontalHeaderItem(0,new QStandardItem(QString("提示")));
        ui->tableView->setModel(emptyModel);
        emptyModel->setItem(0,0,new QStandardItem(QString("没有资产，请添加！")));
        emptyModel->item(0,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableView->verticalHeader()->hide();
        ui->tableView->setColumnWidth(0,820);
        ui->tableView->horizontalHeader()->setSectionsClickable(false);
    }
    else
    {
        m_model = new TableModel();
        ui->tableView->setModel(m_model);
        QStringList headers;
        headers << QString("资产名称") << QString("URL/IP地址")<<
                   QString("扫描工具名称")<<QString("资产类型")<<
                   QString("系统")<<QString("启动工具")<<
                   QString("验证结果");
        m_model->setHorizontalHeader(headers);

        QVector<QStringList> data;
        while(query1.next())
        {
            QString name = query1.value(1).toString();
            QString ip = query1.value(2).toString();
            QString kind = query1.value(3).toString();
            QString sys = query1.value(6).toString();
            QString tool = query1.value(7).toString();

            data.append(QStringList()<<name<<ip<<tool<<kind<<sys);
        }
        m_model->setData(data);
        m_buttonDelegate = new ButtonDelegate(this);
        ui->tableView->setItemDelegateForColumn(5, m_buttonDelegate);
        emit m_model->layoutChanged();

        ui->tableView->verticalHeader()->hide();
        ui->tableView->horizontalHeader()->setSectionsClickable(false);

        ui->tableView->setColumnWidth(1,120);
        ui->tableView->setColumnWidth(2,150);
        ui->tableView->setColumnWidth(3,120);
        ui->tableView->setColumnWidth(0,120);
        ui->tableView->setColumnWidth(4,110);
        ui->tableView->setColumnWidth(5,100);
    }
}
