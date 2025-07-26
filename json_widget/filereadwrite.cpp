#include "filereadwrite.h"
#include "ui_filereadwrite.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QResource>
#include <QStandardPaths>
FileReadWrite::FileReadWrite(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileReadWrite)
{
    ui->setupUi(this);
}

FileReadWrite::~FileReadWrite()
{
    delete ui;
}
void FileReadWrite::change_key(QString key, QString newvalue)
{
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t')+1;
    ui->tableWidget->setRowCount(numRows);
    ui->tableWidget->setColumnCount(numColumns);
    for (int i = 0; i < numRows; ++i)
    {
     QStringList columns = rows[i].split('\t');
     for (int j = 0; j < numColumns; ++j)
        {
        if(j==0)
        {
            if(columns[j]==key)
            {
                ans=1;
                index=i;
            }
        }
        }
    }
    if(ans==1)
    {
        ui->tableWidget->setItem(index,1,new QTableWidgetItem(newvalue));
         ans=0;
         WriteData();
         on_pushButton_clicked();
    }
    else
    {
//         QString key = ui->lineEdit_key->text();
//         QString value = ui->lineEdit_value->text();
//         ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//         int row = ui->tableWidget->rowCount()-1;
//         ui->tableWidget->setItem(row,0,new QTableWidgetItem(key));
//         ui->tableWidget->setItem(row,1,new QTableWidgetItem(value));
//         WriteData();
//         on_pushButton_clicked();
     }
}
void FileReadWrite::on_pushButton_clicked()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/KeysValuesData.txt"));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
    QTextStream in(&file);
    while(!in.atEnd())
    {
        str = in.readAll();
        QStringList rows = str.split('\n');
        int numRows = rows.count();
        int numColumns = rows.first().count('\t')+1;
        ui->tableWidget->setRowCount(numRows);
        ui->tableWidget->setColumnCount(numColumns);
        for (int i = 0; i < numRows; ++i)
        {
            QStringList columns = rows[i].split('\t');
            for (int j = 0; j < numColumns; ++j)
            {
                ui->tableWidget->setItem(i,j,new QTableWidgetItem(columns[j]));
            }
        }
    }
}
void FileReadWrite::WriteData()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/KeysValuesData.txt"));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
     QTextStream stream(&file);
     int n = ui->tableWidget->rowCount();
     int m = ui->tableWidget->columnCount();
      for (int i=0; i<n; ++i)
      {for (int j=0; j<m; j++)
        {
           stream << ui->tableWidget->item(i,j)->text() ;
           if(j!=m-1){stream<< "\t";}
        }
        if(i!=n-1){stream << "\n";}
     }
     file.close();
}
void FileReadWrite::get_no_of_connectors(QString key, QString *Number_of_Connectors)
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/KeysValuesData.txt"));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
    QTextStream in(&file);
    while(!in.atEnd())
    {
        str = in.readAll();
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t')+1;
    ui->tableWidget->setRowCount(numRows);
    ui->tableWidget->setColumnCount(numColumns);
    for (int i = 0; i < numRows; ++i)
    {
     QStringList columns = rows[i].split('\t');
     for (int j = 0; j < numColumns; ++j)
        {
        if(j==0)
        {
            if(columns[j]==key)
            {
                QStringList lines=rows[i].split('\t'); 
                *Number_of_Connectors=lines.at(1);
            }
        }
        }
    }
}
}
void FileReadWrite::insert_list(QString Key, QString Value)
{
      ui->tableWidget->insertRow(ui->tableWidget->rowCount());
      int row = ui->tableWidget->rowCount()-1;
      ui->tableWidget->setItem(row,0,new QTableWidgetItem(Key));
      ui->tableWidget->setItem(row,1,new QTableWidgetItem(Value));
      WriteData();
      on_pushButton_clicked();
}
