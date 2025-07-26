#include "AuthorizeOffline.h"
#include "ui_AuthorizeOffline.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QResource>
#include <QStandardPaths>
AuthorizeOffline::AuthorizeOffline(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AuthorizeOffline)
{
    ui->setupUi(this);
}

AuthorizeOffline::~AuthorizeOffline()
{
    delete ui;
}
void AuthorizeOffline::on_pushButton_clicked()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/AuthorizationData.txt"));

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
    QTextStream in(&file);
    while(!in.atEnd())
    {
        str = in.readAll();
        QStringList rows = str.split('\n');
        int numRows = rows.count();
        getlocalistversion=numRows;
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
void AuthorizeOffline::WriteData()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/AuthorizationData.txt"));
    qDebug() << "filename" << (QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/AuthorizationData.txt"));
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
     file.flush();
     file.close();
}
void AuthorizeOffline::Write_empty()
{
    ui->tableWidget->clear();
}
void AuthorizeOffline::get_list(QString IdTag,QString *ExpiryDate)
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
            if(columns[j]==IdTag)
            {
                QStringList lines=rows[i].split('\t');
                *ExpiryDate=lines.at(1);
            }
        }
        }
    }
}
void AuthorizeOffline::check_list(QString IdTag,int *ans)
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
            if(columns[j]==IdTag)
            {
                qDebug() << rows[i];
                *ans=1;
                index=i;
            }
            else{
                *ans = 0;
            }
        }
        }
    }
}
void AuthorizeOffline::insert_list(QString IdTag, QString ExpiryDate, QString ParentIdTag)
{
      ui->tableWidget->insertRow(ui->tableWidget->rowCount());
      int row = ui->tableWidget->rowCount()-1;
      ui->tableWidget->setItem(row,0,new QTableWidgetItem(IdTag));
      ui->tableWidget->setItem(row,1,new QTableWidgetItem(ExpiryDate));
      ui->tableWidget->setItem(row,2,new QTableWidgetItem(ParentIdTag));
      WriteData();
      on_pushButton_clicked();
}
void AuthorizeOffline::insert_diaglist(QString TimeStamp, QString Status)
{
      ui->tableWidget->insertRow(ui->tableWidget->rowCount());
      int row = ui->tableWidget->rowCount()-1;
      ui->tableWidget->setItem(row,0,new QTableWidgetItem(TimeStamp));
      ui->tableWidget->setItem(row,1,new QTableWidgetItem(Status));
      WriteData();
      loaddata();
}
void AuthorizeOffline::update_list(QString IdTag,QString ExpiryDate,QString ParentIdTag)
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
            if(columns[j]==IdTag)
            {
                qDebug() << rows[i];
                ans=1;
                index=i;
            }
        }
        }
    }
    if(ans==1)
    {
        ui->tableWidget->setItem(index,1,new QTableWidgetItem(ExpiryDate));
        ans=0;
         WriteData();
         on_pushButton_clicked();
    }
    else
    {
         ui->tableWidget->insertRow(ui->tableWidget->rowCount());
         int row = ui->tableWidget->rowCount()-1;
         ui->tableWidget->setItem(row,0,new QTableWidgetItem(IdTag));
         ui->tableWidget->setItem(row,1,new QTableWidgetItem(ExpiryDate));
         ui->tableWidget->setItem(row,2,new QTableWidgetItem(ParentIdTag));
         WriteData();
         on_pushButton_clicked();
     }
}

void AuthorizeOffline::loaddata()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/DiagnosticsData.txt"));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
    QTextStream in(&file);
    while(!in.atEnd())
    {
        str = in.readAll();
        QStringList rows = str.split('\n');
        int numRows = rows.count();
        int numColumns = rows.first().count('\t')+1;
        ui->tableWidget_2->setRowCount(numRows);
        ui->tableWidget_2->setColumnCount(numColumns);
        for (int i = 0; i < numRows; ++i)
        {
            QStringList columns = rows[i].split('\t');
            for (int j = 0; j < numColumns; ++j)
            {
                ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(columns[j]));
            }
        }
    }
}
void AuthorizeOffline::update_list_diag(QString TimeStamp,QString Status)
{
         loaddata();
         ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
         int row = ui->tableWidget_2->rowCount()-1;
         ui->tableWidget_2->setItem(row,0,new QTableWidgetItem(TimeStamp));
         ui->tableWidget_2->setItem(row,1,new QTableWidgetItem(Status));
         WriteDatadiag();
}
void AuthorizeOffline::WriteDatadiag()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/DiagnosticsData.txt"));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
     QTextStream stream(&file);
     int n = ui->tableWidget_2->rowCount();
     int m = ui->tableWidget_2->columnCount();
      for (int i=0; i<n; ++i)
      {for (int j=0; j<m; j++)
        {
           stream << ui->tableWidget_2->item(i,j)->text() ;
           if(j!=m-1){stream<< "\t";}
        }
        if(i!=n-1){stream << "\n";}
     }
     file.close();
}
