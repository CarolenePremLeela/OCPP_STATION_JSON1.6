#include "transactionoffline.h"
#include "ui_transactionoffline.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QResource>
#include <QStandardPaths>
TransactionOffline::TransactionOffline(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TransactionOffline)
{
    ui->setupUi(this);
}

TransactionOffline::~TransactionOffline()
{
    delete ui;
}
void TransactionOffline::on_pushButton_clicked()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/TransactionData.txt"));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
    QTextStream in(&file);
    while(!in.atEnd())
    {
        str = in.readAll();
        qDebug()<<str;
        ui->textEdit->setText(str);
        QStringList rows = str.split('\n');
        int numRows = rows.count();
        int numColumns = rows.first().count('\t')+1;
        ui->tableWidget_1->setRowCount(numRows);
        ui->tableWidget_1->setColumnCount(numColumns);
        for (int i = 0; i < numRows; ++i)
        {
            QStringList columns = rows[i].split('\t');
            for (int j = 0; j < numColumns; ++j)
            {
                ui->tableWidget_1->setItem(i,j,new QTableWidgetItem(columns[j]));
            }
        }
    }
}
void TransactionOffline::WriteData()
{
    QFile file(QStandardPaths::locate(QStandardPaths::DesktopLocation,"Database/TransactionData.txt"));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {QMessageBox::warning(this,tr("Warning!"),tr("          file not found       "));}
     QTextStream stream(&file);
     int n = ui->tableWidget_1->rowCount();
     int m = ui->tableWidget_1->columnCount();
      for (int i=0; i<n; ++i)
      {for (int j=0; j<m; j++)
        {
           stream << ui->tableWidget_1->item(i,j)->text() ;
           if(j!=m-1){stream<< "\t";}
        }
        if(i!=n-1){stream << "\n";}
     }
     file.close();
}
void TransactionOffline::update_list_start(QString TransactionId,QString ConnectorId,QString IdTag,QString MeterStart,QString TimeStamp)
{
         ui->tableWidget_1->insertRow(ui->tableWidget_1->rowCount());
         int row = ui->tableWidget_1->rowCount()-1;
         ui->tableWidget_1->setItem(row,0,new QTableWidgetItem(TransactionId));
         ui->tableWidget_1->setItem(row,1,new QTableWidgetItem(ConnectorId));
         ui->tableWidget_1->setItem(row,2,new QTableWidgetItem(IdTag));
         ui->tableWidget_1->setItem(row,3,new QTableWidgetItem(TimeStamp));
         ui->tableWidget_1->setItem(row,4,new QTableWidgetItem(MeterStart));
         ui->tableWidget_1->setItem(row,5,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,6,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,7,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,8,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,9,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,10,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,11,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,12,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,13,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,14,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,15,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,16,new QTableWidgetItem("0"));
         WriteData();
         on_pushButton_clicked();
}
void TransactionOffline::update_list_metervalues(QString TimeStamp,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit,QString TransactionId)
{
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t')+1;
    ui->tableWidget_1->setRowCount(numRows);
    ui->tableWidget_1->setColumnCount(numColumns);
    for (int i = 0; i < numRows; ++i)
    {
     QStringList columns = rows[i].split('\t');
     for (int j = 0; j < numColumns; ++j)
        {
        if(j==0)
        {
            if(columns[j]==TransactionId)
            {
                ui->tableWidget_1->setItem(i,5,new QTableWidgetItem("0"));
                ui->tableWidget_1->setItem(i,6,new QTableWidgetItem("0"));
                ui->tableWidget_1->setItem(i,7,new QTableWidgetItem(Value));
                ui->tableWidget_1->setItem(i,8,new QTableWidgetItem(Context));
                ui->tableWidget_1->setItem(i,9,new QTableWidgetItem(Format));
                ui->tableWidget_1->setItem(i,10,new QTableWidgetItem(Measured));
                ui->tableWidget_1->setItem(i,11,new QTableWidgetItem(Phase));
                ui->tableWidget_1->setItem(i,12,new QTableWidgetItem(Location));
                ui->tableWidget_1->setItem(i,13,new QTableWidgetItem(Unit));
                ui->tableWidget_1->setItem(i,14,new QTableWidgetItem("0"));
                ui->tableWidget_1->setItem(i,15,new QTableWidgetItem(TimeStamp));
                ui->tableWidget_1->setItem(i,16,new QTableWidgetItem("0"));
                WriteData();
                on_pushButton_clicked();
            }
        }
        }
    }
}

void TransactionOffline::update_list_stop(QString TransactionId,QString ConnectorId,QString IdTag,QString TimeStamp,QString MeterStop,QString Reason,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit)
{
         ui->tableWidget_1->insertRow(ui->tableWidget_1->rowCount());
         int row = ui->tableWidget_1->rowCount()-1;
         ui->tableWidget_1->setItem(row,0,new QTableWidgetItem(TransactionId));
         ui->tableWidget_1->setItem(row,1,new QTableWidgetItem(ConnectorId));
         ui->tableWidget_1->setItem(row,2,new QTableWidgetItem(IdTag));
         ui->tableWidget_1->setItem(row,3,new QTableWidgetItem(TimeStamp));
         ui->tableWidget_1->setItem(row,4,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,5,new QTableWidgetItem(MeterStop));
         ui->tableWidget_1->setItem(row,6,new QTableWidgetItem(Reason));
         ui->tableWidget_1->setItem(row,7,new QTableWidgetItem(Value));
         ui->tableWidget_1->setItem(row,8,new QTableWidgetItem(Context));
         ui->tableWidget_1->setItem(row,9,new QTableWidgetItem(Format));
         ui->tableWidget_1->setItem(row,10,new QTableWidgetItem(Measured));
         ui->tableWidget_1->setItem(row,11,new QTableWidgetItem(Phase));
         ui->tableWidget_1->setItem(row,12,new QTableWidgetItem(Location));
         ui->tableWidget_1->setItem(row,13,new QTableWidgetItem(Unit));
         ui->tableWidget_1->setItem(row,14,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,15,new QTableWidgetItem("0"));
         ui->tableWidget_1->setItem(row,16,new QTableWidgetItem("0"));
         WriteData();
         on_pushButton_clicked();
}
void TransactionOffline::get_list(QString ConnectorId,QString *TransactionId)
{
    QStringList rows = str.split('\n');
    int numRows = rows.count();
    int numColumns = rows.first().count('\t')+1;
    ui->tableWidget_1->setRowCount(numRows);
    ui->tableWidget_1->setColumnCount(numColumns);
    for (int i = 0; i < numRows; ++i)
    {
     QStringList columns = rows[i].split('\t');
     for (int j = 0; j < numColumns; ++j)
        {
        if(j==1)
        {
            if(columns[j]==ConnectorId)
            {
                QStringList lines=rows[i].split('\t');
                *TransactionId=lines.at(0);
            }
        }
        }
    }
}
void TransactionOffline::insert_list(QString TransactionId,QString ConnectorId,QString IdTag,QString TimeStamp,QString MeterStart,QString MeterStop,QString Reason,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit,QString ReservationId,QString MeterTimeStamp,QString Status)
{
         ui->tableWidget_1->insertRow(ui->tableWidget_1->rowCount());
         int row = ui->tableWidget_1->rowCount()-1;
         ui->tableWidget_1->setItem(row,0,new QTableWidgetItem(TransactionId));
         ui->tableWidget_1->setItem(row,1,new QTableWidgetItem(ConnectorId));
         ui->tableWidget_1->setItem(row,2,new QTableWidgetItem(IdTag));
         ui->tableWidget_1->setItem(row,3,new QTableWidgetItem(TimeStamp));
         ui->tableWidget_1->setItem(row,4,new QTableWidgetItem(MeterStart));
         ui->tableWidget_1->setItem(row,5,new QTableWidgetItem(MeterStop));
         ui->tableWidget_1->setItem(row,6,new QTableWidgetItem(Reason));
         ui->tableWidget_1->setItem(row,7,new QTableWidgetItem(Value));
         ui->tableWidget_1->setItem(row,8,new QTableWidgetItem(Context));
         ui->tableWidget_1->setItem(row,9,new QTableWidgetItem(Format));
         ui->tableWidget_1->setItem(row,10,new QTableWidgetItem(Measured));
         ui->tableWidget_1->setItem(row,11,new QTableWidgetItem(Phase));
         ui->tableWidget_1->setItem(row,12,new QTableWidgetItem(Location));
         ui->tableWidget_1->setItem(row,13,new QTableWidgetItem(Unit));
         ui->tableWidget_1->setItem(row,14,new QTableWidgetItem(ReservationId));
         ui->tableWidget_1->setItem(row,15,new QTableWidgetItem(MeterTimeStamp));
         ui->tableWidget_1->setItem(row,16,new QTableWidgetItem(Status));
         WriteData();
         on_pushButton_clicked();
}
