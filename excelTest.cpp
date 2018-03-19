#include <ActiveQt/QAxObject>


void AddDialog::readExcel(QString url)
    {  
        QAxObject excel("Excel.Application");  
        excel.setProperty("Visible",true);  
        QAxObject *work_books = excel.querySubObject("WorkBooks");  
        work_books->dynamicCall("Open (const QString&)",QString(url));  
        QVariant title_value = excel.property("Caption");       //��ȡ������Ϣ
//      qDebug()<<"��ȡExcel����Ϊ��"<<title_value;
        QAxObject *work_book = excel.querySubObject("ActiveWorkBook");  
        QAxObject *work_sheets = work_book->querySubObject("Sheets");    //SheetsҲ���Ի���WorkSheets
        int sheet_count = work_sheets->property("Count").toInt();     //��ȡ��������Ŀ
//      qDebug()<<"��ǰ��������ĿΪ��"<<sheet_count;
        for(int k=1;k<=sheet_count;k++)  
        {  
            QAxObject *work_sheet = work_book->querySubObject("Sheets(int)",k);     //Sheets(int)Ҳ������Worksheets(int)
            QString work_sheet_name = work_sheet->property("Name").toString();      //��ȡ����������
            QString message = QString("Sheet")+QString::number(k,10)+QString("name");  
            //qDebug()<<message<<work_sheet_name;
            if(sheet_count>0)  
            {  
                QAxObject *work_sheet = work_book->querySubObject("Sheets(int)",k);  
                QAxObject *used_range = work_sheet->querySubObject("UsedRange");  
                QAxObject *rows = used_range->querySubObject("Rows");  
              //QAxObject *columns = used_range->querySubObject("Columns");
                int row_start = used_range->property("Row").toInt();    //��ȡ��ʼ��
                int columns_start = used_range->property("Column").toInt();      //��ȡ��ʼ��
                int row_count = rows->property("Count").toInt();        //��ȡ����
              //int column_count = columns->property("Count").toInt();          //��ȡ����
                for(int i=row_start+1;i<=row_count;i++)  
                {  
                    /*********************************** 
                    * ���ݽ����������� 
                    * ��Excel�е����ݷ���ȡ�� 
                    * �ر�ֵ��ע�����
                    * ���������������Ե�ǰʱ��Ϊ����
                    * ����Ҫ������ʱ������ʱ1�� 
                    * ��ֹ����������ͬΪ���ݼ���ʧ�� 
                   ****************************************************************************************************************/  
                    QAxObject *cell[13];  
                    QVariant cell_value[13];  
      
      
                    cell[0] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+0);  
                    cell_value[0] = cell[0]->property("Value");     //��ȡ��Ԫ������
      
      
                    cell[1] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+1);  
                    cell_value[1] = cell[1]->property("Value");  
      
      
                    cell[2] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+2);  
                    cell_value[2] = cell[2]->property("Value");  
      
      
                    cell[3] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+3);  
                    cell_value[3] = cell[3]->property("Value");  
      
      
                    cell[4] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+4);  
                    cell_value[4] = cell[4]->property("Value");  
      
      
                    cell[5] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+5);  
                    cell_value[5] = cell[5]->property("Value");  
      
      
                    cell[6] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+6);  
                    cell_value[6] = cell[6]->property("Value");  
      
      
                    cell[7] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+7);  
                    cell_value[7] = cell[7]->property("Value");  
      
      
                    cell[8] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+8);  
                    cell_value[8] = cell[8]->property("Value");  
      
      
                    cell[9] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+9);  
                    cell_value[9] = cell[9]->property("Value");  
      
      
                    cell[10] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+10);  
                    cell_value[10] = cell[10]->property("Value");  
      
      
                    cell[11] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+11);  
                    cell_value[11] = cell[11]->property("Value");  
      
      
                    cell[12] = work_sheet->querySubObject("Cells(int,int)",i,columns_start+12);  
                    cell_value[12] = cell[12]->property("Value");  
      
      
                    datasave->CRETM = gettimetss();  
                    datasave->pro_name = cell_value[0].toString();  
                    datasave->PSTM = cell_value[1].toString();  
                    datasave->PETM = cell_value[2].toString();  
                    datasave->Par_name = cell_value[3].toString();  
                    datasave->PARSTM = cell_value[4].toString();  
                    datasave->PARETM = cell_value[5].toString();  
                    datasave->Sta_name = cell_value[6].toString();  
                    datasave->STASTM = cell_value[7].toString();  
                    datasave->STAETM = cell_value[8].toString();  
                    datasave->INFOR = cell_value[9].toString();  
                    datasave->DELTM = cell_value[10].toString();  
                    datasave->REASION = cell_value[11].toString();  
                    datasave->FINTM = cell_value[12].toString();  
                    QString sql_insert = "insert into project values('";  
                    sql_insert +=datasave->pro_name;  
                    sql_insert +="','";  
                    sql_insert +=datasave->PSTM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->PETM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->Par_name;  
                    sql_insert +="','";  
                    sql_insert +=datasave->PARSTM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->PARETM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->Sta_name;  
                    sql_insert +="','";  
                    sql_insert +=datasave->STASTM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->STAETM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->INFOR;  
                    sql_insert +="','";  
                    sql_insert +=datasave->FINTM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->DELTM;  
                    sql_insert +="','";  
                    sql_insert +=datasave->REASION;  
                    sql_insert +="','";  
                    sql_insert +=datasave->CRETM;  
                    sql_insert +="')";  
                    QSqlQuery insert_data(*projectdb);  
                    insert_data.exec(sql_insert);  
                    //qDebug()<<sql_insert;
                    emit sys_data();  
                    exceltip->setnumber(i);  
                    delaymsec(10);  
                    /****************************************************************************************************************/  
                }  
            }  
        }  
        exceltip->setfinish(true);  
        if(exceltip->exec() == QDialog::Accepted)  
        {  
            ui->pushButton_Cancle->setEnabled(true);  
            ui->pushButton_close->setEnabled(true);  
            ui->pushButton_save->setEnabled(true);  
            ui->pushButton_Excel->setEnabled(true);  
            ui->pushButton_update->setEnabled(true);  
            getname();  
        }  
    }  
      
      
    /*******************************************************************************/  
      
      
    void AddDialog::on_pushButton_Excel_clicked()     //��һ��Excel�ļ�������Ҫ����Excel��������
    {  
        QString filename = QFileDialog::getOpenFileName(this);  
        if(!filename.isEmpty())  
        {  
            readExcel(filename);  
        }  
    }  
    //�Զ�����ʱ����   //��������ʱ����  
    void AddDialog::delaymsec(int msec)  
    {  
        QTime dieTime = QTime::currentTime().addMSecs(msec);  
        while (QTime::currentTime()<dieTime)  
        {  
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);  
        }  
    }  
