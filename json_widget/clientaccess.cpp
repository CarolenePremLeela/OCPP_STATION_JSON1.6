#include "clientaccess.h"
#include "ui_clientaccess.h"
#include <QDateEdit>
#include <QFileDialog>
#include <QNetworkInterface>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
clientaccess::clientaccess(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::clientaccess)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(myfunction()));
    timer->start(0);
    ui->stackedWidget->setCurrentIndex(12);
    ui->stackedWidget_Registration_Login->setCurrentIndex(0);
    ui->label_Center->setText("Welcome to EV");
    ui->toolButton_next->show();
    ui->toolButton_prev->show();
    ui->label_QR_1->setStyleSheet("image:url(:/icons/TarangEV91.png)");
    ui->label_QR_1->setScaledContents(true);
    ui->label_QR_1->setText("");
}

clientaccess::~clientaccess()
{
    delete ui;
}
void clientaccess::trading()
{
    ui->stackedWidget->setCurrentIndex(9);
    ui->label_Center->setText("Trading");
    QString  StartTime,Rate,Start_MeterValue;
    w2->d.get_starttime_usingconnectorId(&Start_MeterValue,&StartTime,w2->w->ConnectorId);
    ui->lineEdit_Trading_CardId->setText(w2->w->IdTag);
    QDateTime starttime=QDateTime::fromString(StartTime,"yyyy-MM-dd HH:mm:ss");
    QString StartTime_todisplay=starttime.toString(Qt::ISODate);
    ui->lineEdit_Trading_StartTime->setText(StartTime_todisplay);
//    int offset = starttime.offsetFromUtc();
//    starttime.setOffsetFromUtc(offset);
    StartTime=starttime.toString("hh:mm:ss");
    starttime=QDateTime::fromString(StartTime,"hh:mm:ss");
    int starttimesecs=starttime.toMSecsSinceEpoch();

    QDateTime start1=QDateTime::fromString(RatePeriod1_Start,"hh:mm:ss");
    int start1secs=start1.toMSecsSinceEpoch();
    QDateTime stop1=QDateTime::fromString(RatePeriod1_Stop,"hh:mm:ss");
    int stop1secs=stop1.toMSecsSinceEpoch();
    QDateTime start2=QDateTime::fromString(RatePeriod2_Start,"hh:mm:ss");
    int start2secs=start2.toMSecsSinceEpoch();
    QDateTime stop2=QDateTime::fromString(RatePeriod2_Stop,"hh:mm:ss");
    int stop2secs=stop2.toMSecsSinceEpoch();
    QDateTime start3=QDateTime::fromString(RatePeriod3_Start,"hh:mm:ss");
    int start3secs=start3.toMSecsSinceEpoch();
    QDateTime stop3=QDateTime::fromString(RatePeriod3_Stop,"hh:mm:ss");
    int stop3secs=stop3.toMSecsSinceEpoch();
    QDateTime start4=QDateTime::fromString(RatePeriod4_Start,"hh:mm:ss");
    int start4secs=start4.toMSecsSinceEpoch();
    QDateTime stop4=QDateTime::fromString(RatePeriod4_Stop,"hh:mm:ss");
    int stop4secs=stop4.toMSecsSinceEpoch();
    if(starttimesecs>start1secs && starttimesecs<stop1secs)
    {
        Rate=Rate1;
    }
    if(starttimesecs>start2secs && starttimesecs<stop2secs)
    {
        Rate=Rate2;
    }
    if(starttimesecs>start3secs && starttimesecs<stop3secs)
    {
        Rate=Rate3;
    }
    if(starttimesecs>start4secs && starttimesecs<stop4secs)
    {
        Rate=Rate4;
    }
    QDateTime stoptime=QDateTime::fromString(w2->w->TimeStamp,"yyyy-MM-dd HH:mm:ss");
    QString Stop_MeterValue=w2->w->Value;
    qDebug()<<"meter value"<<Stop_MeterValue;
    float startval=Start_MeterValue.toFloat();
    float stopval=Stop_MeterValue.toFloat();
    float ChargeConsumedint=stopval-startval;
    QString ChargeConsumed=QString::number(ChargeConsumedint);
    ui->lineEdit_Trading_ChargeKwh->setText(ChargeConsumed);

//    offset = stoptime.offsetFromUtc();
//    stoptime.setOffsetFromUtc(offset);
    QString StopTime=stoptime.toString(Qt::ISODate);
    ui->lineEdit_Trading_StopTime->setText(StopTime);
    ui->lineEdit_Trading_FareKwh->setText(Rate);
    QString TotalRate=QString::number((ChargeConsumedint* Rate.toInt()));
    ui->lineEdit_Trading_TotalFare->setText(TotalRate);
    QString ServiceFare="1";
    ui->lineEdit_Trading_ServiceFare->setText(ServiceFare);
    w2->d.insert_Trading(w2->w->IdTag,StartTime_todisplay,StopTime,Start_MeterValue,Stop_MeterValue,ChargeConsumed,Rate,TotalRate,ServiceFare);
    w2->w->IdTag="";w2->w->TimeStamp="";w2->w->ConnectorId="";StartTime="";Rate="";TotalRate="";ChargeConsumed="";ServiceFare="";w2->w->Value="";
}
void clientaccess::on_toolButton_Start_Con_1_clicked()
{
    if(w2->w->Authorizevalue==0)
    {
        QMessageBox::warning(this,tr("INFO"),tr("Please Insert a Valid Id"));
        ui->stackedWidget->setCurrentIndex(13);
    }
    else
    {
        w2->w->Authorizevalue=0;
        ui->stackedWidget->setCurrentIndex(13);
        w2->w->on_start_con_1_clicked();
    }
    w2->w->IdTag="";
}

void clientaccess::on_toolButton_Stop_Con_1_clicked()
{    
    if(w2->w->Authorizevalue==0)
    {
        ui->stackedWidget->setCurrentIndex(13);
        QMessageBox::warning(this,tr("INFO"),tr("Please Insert a Valid Id"));
    }
    else
    {
    w2->w->Authorizevalue=0;
    w2->w->on_stop_con_1_clicked();
    if((w2->w->IdTag==w2->w->IdTag_reserve)&&(w2->w->IdTag!="")&&(w2->w->IdTag_reserve!="")&&(w2->w->Value != 0))
    {
        trading();
        w2->w->IdTag="";w2->w->IdTag_reserve="";
    }
    else if(w2->w->Value==0)
    {
        QMessageBox::warning(this,tr("INFO"),tr("Power Meter Fault..Please Try Again..!"));
    }
    else
    {
        QMessageBox::warning(this,tr("INFO"),tr("Please Select valid Connector"));
    }
    }

}

void clientaccess::on_toolButton_Start_Con_2_clicked()
{
    if(w2->w->Authorizevalue==0)
    {
        QMessageBox::warning(this,tr("INFO"),tr("Please Insert a Valid Id"));
        ui->stackedWidget->setCurrentIndex(13);
    }
    else
    {
        w2->w->Authorizevalue=0;
    ui->stackedWidget->setCurrentIndex(13);
    w2->w->on_start_con_2_clicked();
    }
    w2->w->IdTag="";
}

void clientaccess::on_toolButton_Stop_Con_2_clicked()
{
    if(w2->w->Authorizevalue==0)
    {
        ui->stackedWidget->setCurrentIndex(13);
        QMessageBox::warning(this,tr("INFO"),tr("Please Insert a Valid Id"));
    }
    else
    {
    w2->w->Authorizevalue=0;
    w2->w->on_stop_con_2_clicked();
    if((w2->w->IdTag==w2->w->IdTag_reserve)&&(w2->w->IdTag!="")&&(w2->w->IdTag_reserve!="")&&(w2->w->Value != 0))
    {
        trading();
        w2->w->IdTag="";w2->w->IdTag_reserve="";
    }
    else if(w2->w->Value==0)
    {
        QMessageBox::warning(this,tr("INFO"),tr("Power Meter Fault..Please Try Again..!"));
    }
    else
    {
        QMessageBox::warning(this,tr("INFO"),tr("Please Select valid Connector"));
    }
    }
}
void clientaccess::on_toolButton_Start_Con_3_clicked()
{
    if(w2->w->Authorizevalue==0)
    {
        QMessageBox::warning(this,tr("INFO"),tr("Please Insert a Valid Id"));
        ui->stackedWidget->setCurrentIndex(13);
    }
    else
    {
        w2->w->Authorizevalue=0;
        ui->stackedWidget->setCurrentIndex(13);
        w2->w->on_start_con_3_clicked();
    }
    w2->w->IdTag="";
}
void clientaccess::on_toolButton_Stop_Con_3_clicked()
{
    if(w2->w->Authorizevalue==0)
    {
        ui->stackedWidget->setCurrentIndex(13);
        QMessageBox::warning(this,tr("INFO"),tr("Please Insert a Valid Id"));
    }
    else
    {
    w2->w->Authorizevalue=0;
    w2->w->on_stop_con_3_clicked();
    if((w2->w->IdTag==w2->w->IdTag_reserve)&&(w2->w->IdTag!="")&&(w2->w->IdTag_reserve!="")&&(w2->w->Value != 0))
    {
        trading();
        w2->w->IdTag="";w2->w->IdTag_reserve="";
    }
    else if(w2->w->Value==0)
    {
        QMessageBox::warning(this,tr("INFO"),tr("Power Meter Fault..Please Try Again..!"));

    }
    else
    {
        QMessageBox::warning(this,tr("INFO"),tr("Please Select valid Connector"));
    }
    }
}

void clientaccess::myfunction()
{
    switch (w2->w->No_of_Connectors) {
    case 1:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->hide();
        ui->toolButton_Stop_Con_2->hide();
        ui->toolButton_Start_Con_3->hide();
        ui->toolButton_Stop_Con_3->hide();
        break;
    case 2:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->show();
        ui->toolButton_Stop_Con_2->show();
        ui->toolButton_Start_Con_3->hide();
        ui->toolButton_Stop_Con_3->hide();
        break;
    case 3:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->show();
        ui->toolButton_Stop_Con_2->show();
        ui->toolButton_Start_Con_3->show();
        ui->toolButton_Stop_Con_3->show();
        break;
    case 4:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->show();
        ui->toolButton_Stop_Con_2->show();
        ui->toolButton_Start_Con_3->show();
        ui->toolButton_Stop_Con_3->show();
        break;
    case 5:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->show();
        ui->toolButton_Stop_Con_2->show();
        ui->toolButton_Start_Con_3->show();
        ui->toolButton_Stop_Con_3->show();
        break;
    case 6:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->show();
        ui->toolButton_Stop_Con_2->show();
        ui->toolButton_Start_Con_3->show();
        ui->toolButton_Stop_Con_3->show();
        break;
    case 7:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->show();
        ui->toolButton_Stop_Con_2->show();
        ui->toolButton_Start_Con_3->show();
        ui->toolButton_Stop_Con_3->show();
        break;
    case 8:
        ui->toolButton_Start_Con_1->show();
        ui->toolButton_Stop_Con_1->show();
        ui->toolButton_Start_Con_2->show();
        ui->toolButton_Stop_Con_2->show();
        ui->toolButton_Start_Con_3->show();
        ui->toolButton_Stop_Con_3->show();
        break;
    default:
        break;
    }
  if(w2->w->avail[0]==0)
   {
       // If Connector1 is not available - Black Indication
       ui->toolButton_Start_Con_1->setStyleSheet("background-color:black;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Stop_Con_1->setStyleSheet("background-color:black;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Start_Con_1->setText("Not Available");
       ui->toolButton_Stop_Con_1->setText("Not Available");
    }
    else
   {
      ui->toolButton_Start_Con_1->setStyleSheet("background-color:green;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
      ui->toolButton_Stop_Con_1->setStyleSheet("background-color:red;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
      if(w2->w->mON[0]==1)
      {
           // If Connector1 available and Meter is ON - Green Indication
           ui->toolButton_Start_Con_1->setEnabled(false);
           ui->toolButton_Start_Con_1->setStyleSheet("background-color:grey;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
           ui->toolButton_Stop_Con_1->setEnabled(true);
           ui->toolButton_Start_Con_1->setText("StartConnector1");
           ui->toolButton_Stop_Con_1->setText("StopConnector1");
           //ui->frame_QR_1->setStyleSheet("background-color:green;border-radius:25px;");
      }
      else if(w2->w->res[0]==1)
      {
          ui->toolButton_Start_Con_1->setEnabled(false);
          ui->toolButton_Start_Con_1->setStyleSheet("background-color:Orange;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
          ui->toolButton_Stop_Con_1->setStyleSheet("background-color:Orange;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
          ui->toolButton_Start_Con_1->setText("Reserved");
          ui->toolButton_Stop_Con_1->setText("Reserved");
          ui->toolButton_Start_Con_1->setDisabled(true);
          ui->toolButton_Stop_Con_1->setDisabled(true);
      }
      else
      {
           // If Connector1 available and Meter is OFF - Red Indication
           ui->toolButton_Start_Con_1->setText("StartConnector1");
           ui->toolButton_Stop_Con_1->setText("StopConnector1");
           ui->toolButton_Start_Con_1->setEnabled(true);
           ui->toolButton_Stop_Con_1->setEnabled(false);
           ui->toolButton_Stop_Con_1->setStyleSheet("background-color:grey;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       }
   }
   if(w2->w->avail[1]==0)
   {
       // If Connector1 is not available - Black Indication
       ui->toolButton_Start_Con_2->setStyleSheet("background-color:black;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Stop_Con_2->setStyleSheet("background-color:black;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Start_Con_2->setText("Not Available");
       ui->toolButton_Stop_Con_2->setText("Not Available");
   }
   else
   {
       ui->toolButton_Start_Con_2->setStyleSheet("background-color:green;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Stop_Con_2->setStyleSheet("background-color:red;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       if(w2->w->mON[1]==1)
       {
           // If Connector1 available and Meter is ON - Green Indication
           ui->toolButton_Start_Con_2->setEnabled(false);
           ui->toolButton_Start_Con_2->setStyleSheet("background-color:grey;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
           ui->toolButton_Stop_Con_2->setEnabled(true);
           ui->toolButton_Start_Con_2->setText("StartConnector2");
           ui->toolButton_Stop_Con_2->setText("StopConnector2");
       }
       else if(w2->w->res[1]==1)
       {
           ui->toolButton_Start_Con_2->setStyleSheet("background-color:Orange;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
           ui->toolButton_Stop_Con_2->setStyleSheet("background-color:Orange;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
           ui->toolButton_Start_Con_2->setText("Reserved");
           ui->toolButton_Stop_Con_2->setText("Reserved");
           ui->toolButton_Start_Con_2->setDisabled(true);
           ui->toolButton_Stop_Con_2->setDisabled(true);
       }
       else
       {
           // If Connector1 available and Meter is OFF - Red Indication
           ui->toolButton_Start_Con_2->setText("StartConnector2");
           ui->toolButton_Stop_Con_2->setText("StopConnector2");
           ui->toolButton_Start_Con_2->setEnabled(true);
           ui->toolButton_Stop_Con_2->setEnabled(false);
          ui->toolButton_Stop_Con_2->setStyleSheet("background-color:grey;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
        }
    }
   if(w2->w->avail[2]==0)
   {
       // If Connector1 is not available - Black Indication
       ui->toolButton_Start_Con_3->setStyleSheet("background-color:black;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Stop_Con_3->setStyleSheet("background-color:black;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Start_Con_3->setText("Not Available");
       ui->toolButton_Stop_Con_3->setText("Not Available");
   }
   else
   {
       ui->toolButton_Start_Con_3->setStyleSheet("background-color:green;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       ui->toolButton_Stop_Con_3->setStyleSheet("background-color:red;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       if(w2->w->mON[2]==1)
       {
           // If Connector1 available and Meter is ON - Green Indication
           ui->toolButton_Start_Con_3->setEnabled(false);
           ui->toolButton_Start_Con_3->setStyleSheet("background-color:grey;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
           ui->toolButton_Stop_Con_3->setEnabled(true);
           ui->toolButton_Start_Con_3->setText("StartConnector3");
           ui->toolButton_Stop_Con_3->setText("StopConnector3");
       }
       else if(w2->w->res[2]==1)
       {
           ui->toolButton_Start_Con_3->setStyleSheet("background-color:Orange;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
           ui->toolButton_Stop_Con_3->setStyleSheet("background-color:Orange;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
           ui->toolButton_Start_Con_3->setText("Reserved");
           ui->toolButton_Stop_Con_3->setText("Reserved");
           ui->toolButton_Start_Con_3->setDisabled(true);
           ui->toolButton_Stop_Con_3->setDisabled(true);
       }
       else
       {
           // If Connector1 available and Meter is OFF - Red Indication
           ui->toolButton_Start_Con_3->setText("StartConnector3");
           ui->toolButton_Stop_Con_3->setText("StopConnector3");
           ui->toolButton_Start_Con_3->setEnabled(true);
           ui->toolButton_Stop_Con_3->setEnabled(false);
           ui->toolButton_Stop_Con_3->setStyleSheet("background-color:grey;border: 1px solid black;border-radius: 19px;color:white;font-size: 15px");
       }
   }
   QDateTime issueDate = QDateTime::currentDateTime();
   QString gmt=issueDate.toString("yyyy-MM-dd HH:mm:ss");
   ui->lcdNumber_CurrentDate->display(gmt);
   // Check Network connectivity
   QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
   bool tempresult = false;
   for (int i = 0; i < ifaces.count(); i++)
   {
       QNetworkInterface iface = ifaces.at(i);
       if ( iface.flags().testFlag(QNetworkInterface::IsUp)
            && !iface.flags().testFlag(QNetworkInterface::IsLoopBack) )
       {

           for (int j=0; j<iface.addressEntries().count(); j++)
           {
               if (tempresult == false)
                   tempresult = true;
           }
       }

   }
   w2->w->networkresult=tempresult;
    // Check Network
   qDebug()<<"status="<<w2->w->networkresult;
    if((w2->w->networkresult==true))
        {
        netcount=1;
        ui->label_Network->setText("Online");
        serial.start();
        if((serial.size<9 && serial.size>0)&&(count==1))
        {
            on_pushButton_failure_clicked(gmt,"Power Meter Failure","104");
            count=0;
        }
        else if((serial.size==0)&&(count==1))
        {
            on_pushButton_failure_clicked(gmt,"Communication Failure","106");
            count=0;
        }
        else if((serial.size>=9)&&(count==1))
        {
            if((serial.Voltage<232.00)&&(count==1))
            {
                on_pushButton_failure_clicked(gmt,"Over Voltage","107");
                count=0;
            }
            else
            {
                count=1;
            }
            if((serial.Current>1.5)&&(count==1))
            {
                on_pushButton_failure_clicked(gmt,"Over Current","109");
                count=0;
            }
            else
            {
                count=1;
            }
//            if(dial->Power_Switch_Failure=="1")
//            {
//                QMessageBox::StandardButton reply=QMessageBox::warning(this,tr("Power Meter Failure"),tr("No Meter Reading"
//                                                                       "Out Of Service"),QMessageBox::Yes|QMessageBox::No);
//                if(reply == QMessageBox::Yes)
//                {
//    //                qApp->quit();
//    //                QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
//                }
//    //            acc->w2->w->StatusNotification();
//            }
//            if(dial->Temperature_Value=="1")
//            {
//                QMessageBox::warning(this,tr("Over temperature"),tr("voltage exceeds 300v"));
//            }
//            if(dial->Ground_Failure=="1")
//            {
//                QMessageBox::warning(this,tr("Power Switch Failure"),tr("Low voltage Supply"));
//            }
//            if(dial->Connector_Lock_Failure=="1")
//            {
//                QMessageBox::warning(this,tr("Connector Lock Failure"),tr("Low voltage Supply"));
//            }
//            if(dial->Reader_Failure=="1")
//            {
//                QMessageBox::warning(this,tr("Reader Failure"),tr("voltage exceeds 30amps"));
//            }
//            if(dial->Ground_Failure=="1")
//            {
//                QMessageBox::warning(this,tr("Ground Failure"),tr("Due to Ground Failure System is going to SHUTDOWN Now"));
//                QTimer *timer;
//                timer=new QTimer(this);
//                timer->start(2000);
//    //            QProcess::startDetached("shutdown /l");
//            }
        }
        }
    else if((w2->w->networkresult==false)&&(netcount==1))
        {
                on_pushButton_failure_clicked(gmt,"Weak Signal","112");
        ui->label_Network->setText("Offline");
        netcount=0;
        }
//    QProcess pingProcess;
//    QString exe = "ping google.com";
//    pingProcess.start(exe);

//    pingProcess.waitForFinished();
//    QString outputinms = pingProcess.readAllStandardOutput();
//    QStringList Output1= outputinms.split("\r\n");
//    QString out = Output1.at(10);
//    QStringList outcomspace = out.split(", ");
//    QString outaverage = outcomspace.at(2);
//    QStringList outequal = outaverage.split("= ");
//    QString output = outequal.at(1);
//    QStringList speed = output.split("ms");
//    QString Internetspeed = speed.at(0);
//    double link = Internetspeed.toDouble();
//    double Networkspeed = (32*1000)/(link*125); // (32Bytes pinging diving with pinging time in ms....Then covert into KB by dividing with 125)
//    if(Networkspeed < 5)
//    {
//        QMessageBox::warning(this, tr("Weak Signal"), tr("Network Signal Streangth is Weak"));
//        on_pushButton_failure_clicked(gmt,"Weak Signal","112");
//    }else{
//       qDebug() << "NetworkSpeed" << Networkspeed << "KB/s";
//    }
    if((ui->lineEdit_MeterSerialNo->text().isEmpty()==false)&&(ui->lineEdit_ClientName->text().isEmpty()==false)&&(ui->lineEdit_MailId->text().isEmpty()==false)&&(ui->lineEdit_Address->text().isEmpty()==false)&&(ui->lineEdit_MobileNo->text().isEmpty()==false)&&(ui->lineEdit_LandlineNo->text().isEmpty()==false)&&(ui->lineEdit_Password->text().isEmpty()==false)&&(ui->lineEdit_ReenterPassword->text().isEmpty()==false))
    {
        ui->toolButton_register_submit->setStyleSheet("background-color:green;border: 1px solid black;border-radius: 15px;color:white;font-size: 15px");
        ui->toolButton_register_submit->setEnabled(true);
    }
    else
    {
        ui->toolButton_register_submit->setStyleSheet("background-color:grey;border: 1px solid black;border-radius: 15px;color:white;font-size: 15px");
        ui->toolButton_register_submit->setEnabled(false);
    }
}
void clientaccess::get_Idtag(QString IdTag)
{
    ui->lineEdit_Idtag->setText(IdTag);
}
//void clientaccess::on_toolButton_Confirm_IPSet_clicked()
//{
//   w2->w->IdTag=ui->lineEdit_Idtag->text();
//   w2->w->on_toolButton_Send_Authorization_Req_clicked();
//   ui->stackedWidget->setCurrentIndex(1);
//   ui->label_Center->setText("Progress Window");
//}
//void clientaccess::on_toolButton_Cancel_clicked()
//{
//    ui->stackedWidget->setCurrentIndex(13);
//    ui->toolButton_Settings->show();
//    ui->toolButton_Info->show();
//    ui->label_Center->setText("WelCome");
//}
void clientaccess::on_toolButton_gotoregister_clicked()
{
    ui->stackedWidget_Registration_Login->setCurrentIndex(1);
}

void clientaccess::on_toolButton_gotologin_clicked()
{
    ui->stackedWidget_Registration_Login->setCurrentIndex(4);
}

void clientaccess::on_toolButton_register_submit_clicked()
{
        QString MeterSerialNo=ui->lineEdit_MeterSerialNo->text();
        QString ClientName=ui->lineEdit_ClientName->text();
        QString CompanyName=ui->lineEdit_CompanyName->text();
        QString ClientMailId=ui->lineEdit_MailId->text();
        QString ClientAddress=ui->lineEdit_Address->text();
        QString ClientPincode=ui->lineEdit_pincode->text();
        QString ClientMobileNo=ui->lineEdit_MobileNo->text();
        QString ClientLandLineNo=ui->lineEdit_LandlineNo->text();
        QString ClientPassword=ui->lineEdit_Password->text();
        QString ReEnter_Password=ui->lineEdit_ReenterPassword->text();
        QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
        mailREX.setCaseSensitivity(Qt::CaseInsensitive);
        mailREX.setPatternSyntax(QRegExp::RegExp);
        QRegExp mobileREX("\\d{10}");
        QRegExp pincodeREX("\\d{6}");
        QRegExp landlineREX("\\d{10}");
        w2->w->d.get_clientidforregistration(&ClientId,MeterSerialNo);
        if(ClientId==NULL)
        {
            ui->lineEdit_MeterSerialNo->clear();
            QMessageBox::information(this,tr("Info"),tr("Please enter valid Meter Serial Number"));
        }
        else
        {
            if(ClientPincode==NULL && pincodeREX.exactMatch(ClientPincode)==false)
            {
                ui->lineEdit_pincode->clear();
                QMessageBox::information(this,tr("Info"),tr("Please enter Pincode"));
            }
            else
            {
            if(mobileREX.exactMatch(ClientMobileNo)==false)
            {
                ui->lineEdit_MobileNo->clear();
                QMessageBox::information(this,tr("Info"),tr("Please re-enter the Mobile Number"));
            }
            else
            {
                if(landlineREX.exactMatch(ClientLandLineNo)==false)
                {
                    ui->lineEdit_LandlineNo->clear();
                    QMessageBox::information(this,tr("Info"),tr("Please re-enter the LandLine Number \n LandLine Format = pincodeYourNo"));
                }
                else
                {
                    if(mailREX.exactMatch(ClientMailId)==false)
                    {
                        ui->lineEdit_MailId->clear();
                        QMessageBox::information(this,tr("Info"),tr("Please re-enter the MailId"));
                    }
                    else
                    {
                        int passwordLen = ui->lineEdit_Password->text().length();
                        if(passwordLen < 8)
                        {
                            ui->label_passwordstre->setText("Your password must be at least 8 characters\n");
                        }
                        else
                        {
                            ui->label_passwordstre->setText("Your password is good");
                            if(ClientPassword==ReEnter_Password)
                            {
                                w2->w->d.registration_insert(ClientId,ClientName,CompanyName,ClientMailId,ClientAddress,ClientPincode,ClientMobileNo,ClientLandLineNo,ClientPassword);
                                ui->stackedWidget_Registration_Login->setCurrentIndex(2);
                                ui->lineEdit_login_ClienId->setText(ClientId);

                            }
                            else
                            {
                                ui->lineEdit_ReenterPassword->clear();
                                QMessageBox::information(this,tr("Info"),tr("Please re-enter the Password"));
                            }
                        }
                    }
                }
            }
          }
        }
}
void clientaccess::on_toolButton_register_cancel_clicked()
{
    ui->stackedWidget_Registration_Login->setCurrentIndex(0);
    ui->lineEdit_MeterSerialNo->clear();
    ui->lineEdit_ClientName->clear();
    ui->lineEdit_CompanyName->clear();
    ui->lineEdit_MailId->clear();
    ui->lineEdit_Address->clear();
    ui->lineEdit_pincode->clear();
    ui->lineEdit_MobileNo->clear();
    ui->lineEdit_LandlineNo->clear();
    ui->lineEdit_Password->clear();
    ui->lineEdit_ReenterPassword->clear();
}

void clientaccess::on_toolButton_Client_Login_clicked()
{
    ClientId=ui->lineEdit_login_ClienId->text();
    QString Password = ui->lineEdit_login_Password->text();
    QString password;
    w2->w->d.get_passwordforlogin(&password,ClientId);
    if((password==Password)&&(Password!=NULL)&&(ClientId!=NULL))
    {
        ui->stackedWidget->setCurrentIndex(13);
        ui->lineEdit_login_ClienId->clear();
        ui->lineEdit_login_Password->clear();
    }
    else
    {
        QMessageBox::information(this,tr("Info"),tr("Please Enter Correct Password"));
        ui->lineEdit_login_Password->clear();
    }

}

void clientaccess::on_toolButton_Client_Login_Cancel_clicked()
{
    ui->stackedWidget_Registration_Login->setCurrentIndex(0);
}

void clientaccess::on_toolButton_Confirm_IPConfig_clicked()
{
    QString ServeIP=ui->lineEdit_Server_IP->text();
    QString ServerPort=ui->lineEdit_Server_Port->text();
    QString MaCAddress=ui->lineEdit_MacAddress->text();
    QString Domain=ui->lineEdit_Domain->text();
    QString IPMode=ui->comboBox_IPMode->currentText();
    QString LocalIP,GatewayIPAddress,Netmask,DNS;
    if(IPMode=="Auto")
    {
        foreach(const QHostAddress &address,QNetworkInterface::allAddresses())
        {
            if(address.protocol()==QAbstractSocket::IPv4Protocol && address !=QHostAddress(QHostAddress::LocalHost))
            {
                  LocalIP= address.toString();
             }
         }
         foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
             foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
                 if (entry.ip().toString() == LocalIP) {
//                     localMacAddress = networkInterface.hardwareAddress();
                     Netmask = entry.netmask().toString();
                     break;
                 }
             }
         }
    }
    if(IPMode=="Manual")
    {
        LocalIP=ui->lineEdit_LocalIP->text();
        GatewayIPAddress=ui->lineEdit_GwIPAddr->text();
        Netmask=ui->lineEdit_NetMask->text();
        DNS=ui->lineEdit_DNS->text();
    }
w2->d.update_clientip(LocalIP,ClientId);
// Automatically Fetching IP from Localhost System
foreach(const QHostAddress &address,QNetworkInterface::allAddresses())
{
    if(address.protocol()==QAbstractSocket::IPv4Protocol && address !=QHostAddress(QHostAddress::LocalHost))
    {
        w2->w->ip=address.toString();
    }
}
w2->d.get_client_id(&w2->w->ClientId,w2->w->ip);
w2->d.get_connectorids(&w2->w->ConnectorId,w2->w->ip); // Get Connectorids from connectorid details table using clientid
w2->d.get_NoofConnectors_usingclienttable(&w2->w->No_of_Connectors,w2->w->ClientId);// Get Number of Connectors from data(client_table)
//     Split all Connectorids individually and stored in Array
QStringList Connector=w2->w->ConnectorId.split('/');
//No_of_Connectors=Connector.count();
for(int i=0;i<w2->w->No_of_Connectors;i++)
{
    w2->w->Connectors[i]=Connector.at(i+1);
}
w2->w->timer->start(0);
w2->d.get_interval(w2->w->ClientId,&w2->w->interval);
w2->w->timer2->start(w2->w->interval*3600*1000);
QString clientId;
QString managerId;
w2->w->d.get_clientid_using_IP(&clientId,LocalIP);
w2->w->d.get_managerid_using_clientid(&managerId,clientId);
ui->lineEdit_ClientID->setText(clientId);
ui->lineEdit_ManagerID->setText(managerId);
ui->stackedWidget->setCurrentIndex(13);
}

void clientaccess::on_toolButton_Cancel_IPConfig_clicked()
{
        ui->stackedWidget->setCurrentIndex(3);
}
void clientaccess::on_toolButton_Client_Login_withoutreg_clicked()
{
    QString MeterSerialNo=ui->lineEdit_login_withoutreg_MtrSNo->text();
    QString Password = ui->lineEdit_login_withoutreg_password->text();
    QString password;
    w2->w->d.get_clientidforregistration(&ClientId,MeterSerialNo);
    w2->w->d.get_passwordforlogin(&password,ClientId);
    if((password==Password)&&(Password!=NULL)&&(ClientId!=NULL))
    {
        //QMessageBox::information(this,tr("Info"),ClientId);
        ui->stackedWidget_Registration_Login->setCurrentIndex(5);
        ui->lineEdit_login_withoutreg_MtrSNo->clear();
        ui->lineEdit_login_withoutreg_password->clear();
    }
    else
    {
        QMessageBox::information(this,tr("Info"),tr("Please Enter Correct Password"));
        ui->lineEdit_login_withoutreg_password->clear();
    }
}

void clientaccess::on_toolButton_Client_Login_Cancel_withoutreg_clicked()
{
    ui->stackedWidget_Registration_Login->setCurrentIndex(0);
    ui->lineEdit_login_withoutreg_MtrSNo->clear();
    ui->lineEdit_login_withoutreg_password->clear();
}
void clientaccess::on_toolButton_prev_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    qDebug()<<index;
    if(index==12)
    {
        int regindex=ui->stackedWidget_Registration_Login->currentIndex();
        if(regindex==0)
        {
            ui->stackedWidget_Registration_Login->setCurrentIndex(3);
        }
        else if(regindex==3)
        {
            ui->stackedWidget->setCurrentIndex(index-1);
        }
        else
        {
            ui->stackedWidget_Registration_Login->setCurrentIndex(regindex-1);
        }
    }
    else
    {
        if(index!=0)
        {
            ui->stackedWidget->setCurrentIndex(index-1);
        }
        if(index==0)
        {
            ui->stackedWidget->setCurrentIndex(12);
            ui->stackedWidget_Registration_Login->setCurrentIndex(0);
        }
    }
}

void clientaccess::on_toolButton_next_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    qDebug()<<index;
    if(index==12)
    {
        int regindex=ui->stackedWidget_Registration_Login->currentIndex();
        if(regindex==3)
        {
            ui->stackedWidget->setCurrentIndex(13);
        }
        else
        {
            ui->stackedWidget_Registration_Login->setCurrentIndex(regindex+1);
        }
    }
    else
    {
        if(index!=12)
        {
            ui->stackedWidget->setCurrentIndex(index+1);
        }
        if(index==11)
        {
            ui->stackedWidget_Registration_Login->setCurrentIndex(0);
        }
        if(index==12)
        {
            ui->stackedWidget->setCurrentIndex(13);
        }
    }
}

void clientaccess::on_toolButton_AC_Rate_clicked()
{
    ui->stackedWidget_ACDCRate->setCurrentIndex(0);
}

void clientaccess::on_toolButton_DC_Rate_clicked()
{
    ui->stackedWidget_ACDCRate->setCurrentIndex(1);
}

void clientaccess::on_toolButton_DC_Enter_clicked()
{
    QString DC_00=ui->lineEdit_DC_00->text();
    QString DC_01=ui->lineEdit_DC_01->text();
    RatePeriod1_Start=DC_00+":"+DC_01+":00";
    QString DC_02=ui->lineEdit_DC_02->text();
    QString DC_03=ui->lineEdit_DC_03->text();
    RatePeriod1_Stop=DC_02+":"+DC_03+":00";
    Rate1=ui->lineEdit_DC_04->text();
    QString DC_10=ui->lineEdit_DC_10->text();
    QString DC_11=ui->lineEdit_DC_11->text();
    RatePeriod2_Start=DC_10+":"+DC_11+":00";
    QString DC_12=ui->lineEdit_DC_12->text();
    QString DC_13=ui->lineEdit_DC_13->text();
    RatePeriod2_Stop=DC_12+":"+DC_13+":00";
    Rate2=ui->lineEdit_DC_14->text();
    QString DC_20=ui->lineEdit_DC_20->text();
    QString DC_21=ui->lineEdit_DC_21->text();
    RatePeriod3_Start=DC_20+":"+DC_21+":00";
    QString DC_22=ui->lineEdit_DC_22->text();
    QString DC_23=ui->lineEdit_DC_23->text();
    RatePeriod3_Stop=DC_22+":"+DC_23+":00";
    Rate3=ui->lineEdit_DC_24->text();
    QString DC_30=ui->lineEdit_DC_30->text();
    QString DC_31=ui->lineEdit_DC_31->text();
    RatePeriod4_Start=DC_30+":"+DC_31+":00";
    QString DC_32=ui->lineEdit_DC_32->text();
    QString DC_33=ui->lineEdit_DC_33->text();
    RatePeriod4_Stop=DC_32+":"+DC_33+":00";
    Rate4=ui->lineEdit_DC_34->text();
    QDateTime issueDate = QDateTime::currentDateTime();
    QString CurrentDate=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    w2->d.insert_Rate_Settings(ClientId,"DC",RatePeriod1_Start,RatePeriod1_Stop,RatePeriod2_Start,RatePeriod2_Stop,RatePeriod3_Start,RatePeriod3_Stop,RatePeriod4_Start,RatePeriod4_Stop,Rate1,Rate2,Rate3,Rate4,CurrentDate);
    ui->stackedWidget->setCurrentIndex(3);
}
void clientaccess::on_toolButton_AC_Enter_clicked()
{
    QString AC_00=ui->lineEdit_AC_00->text();
    QString AC_01=ui->lineEdit_AC_01->text();
    RatePeriod1_Start=AC_00+":"+AC_01+":00";
    QString AC_02=ui->lineEdit_AC_02->text();
    QString AC_03=ui->lineEdit_AC_03->text();
    RatePeriod1_Stop=AC_02+":"+AC_03+":00";
    Rate1=ui->lineEdit_AC_04->text();
    QString AC_10=ui->lineEdit_AC_10->text();
    QString AC_11=ui->lineEdit_AC_11->text();
    RatePeriod2_Start=AC_10+":"+AC_11+":00";
    QString AC_12=ui->lineEdit_AC_12->text();
    QString AC_13=ui->lineEdit_AC_13->text();
    RatePeriod2_Stop=AC_12+":"+AC_13+":00";
    Rate2=ui->lineEdit_AC_14->text();
    QString AC_20=ui->lineEdit_AC_20->text();
    QString AC_21=ui->lineEdit_AC_21->text();
    RatePeriod3_Start=AC_20+":"+AC_21+":00";
    QString AC_22=ui->lineEdit_AC_22->text();
    QString AC_23=ui->lineEdit_AC_23->text();
    RatePeriod3_Stop=AC_22+":"+AC_23+":00";
    Rate3=ui->lineEdit_AC_24->text();
    QString AC_30=ui->lineEdit_AC_30->text();
    QString AC_31=ui->lineEdit_AC_31->text();
    RatePeriod4_Start=AC_30+":"+AC_31+":00";
    QString AC_32=ui->lineEdit_AC_32->text();
    QString AC_33=ui->lineEdit_AC_33->text();
    RatePeriod4_Stop=AC_32+":"+AC_33+":00";
    Rate4=ui->lineEdit_AC_34->text();
    QDateTime issueDate = QDateTime::currentDateTime();
    QString CurrentDate=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    w2->w->d.insert_Rate_Settings(ClientId,"AC",RatePeriod1_Start,RatePeriod1_Stop,RatePeriod2_Start,RatePeriod2_Stop,RatePeriod3_Start,RatePeriod3_Stop,RatePeriod4_Start,RatePeriod4_Stop,Rate1,Rate2,Rate3,Rate4,CurrentDate);
    ui->stackedWidget->setCurrentIndex(3);
}
void clientaccess::on_toolButton_IdTag_Confirm_clicked()
{
    w2->w->IdTag=ui->lineEdit_Idtag->text();
    w2->w->on_pushButton_Send_Authorization_Req_clicked();
    if((w2->w->IdTag!=""))
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void clientaccess::on_toolButton_Trading_Return_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
    ui->lineEdit_Trading_CardId->clear();
    ui->lineEdit_Trading_StartTime->clear();
    ui->lineEdit_Trading_StopTime->clear();
    ui->lineEdit_Trading_ChargeKwh->clear();
    ui->lineEdit_Trading_FareKwh->clear();
    ui->lineEdit_Trading_TotalFare->clear();
    ui->lineEdit_Trading_ServiceFare->clear();
}

void clientaccess::on_pushButton_failure_clicked(QString GMT,QString Fault,QString Fault_Code)
{
    ui->tableWidget_fault->insertRow(ui->tableWidget_fault->rowCount());
    int row = ui->tableWidget_fault->rowCount()-1;
    ui->tableWidget_fault->setItem(row,0,new QTableWidgetItem(GMT));
    ui->tableWidget_fault->setItem(row,1,new QTableWidgetItem(Fault));
    ui->tableWidget_fault->setItem(row,2,new QTableWidgetItem(Fault_Code));
}

void clientaccess::on_toolButton_Fault_Export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export Contact"), "",
        tr("text files(*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
//! [export function part1]

//! [export function part2]
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        return;
    }

   // QTextStream out(&file);
//! [export function part2]
     QTextStream stream(&file);
     stream<<"S.no"<<"\t"<<"Occured_time"<<"\t"<<"Fault_Code"<<"\t"<<"Fault_Details"<<"\n";
     stream<<1<<"\t";
     int n = ui->tableWidget_fault->rowCount();
     int m = ui->tableWidget_fault->columnCount();
      for (int i=0; i<n; ++i)
      {for (int j=0; j<m; j++)
        {
           stream << ui->tableWidget_fault->item(i,j)->text() ;
           if(j!=m-1){stream<< "\t";}
        }
          qDebug()<<ui->tableWidget_fault->item(i,0)->text()<<ui->tableWidget_fault->item(i,1)->text()<<ui->tableWidget_fault->item(i,2)->text();
          w2->d.insert_faultreport(ui->tableWidget_fault->item(i,0)->text(),ui->tableWidget_fault->item(i,1)->text(),ui->tableWidget_fault->item(i,2)->text());
          if(i!=n-1){stream<< "\n"<<i+2<<"\t";
        }
     }
     file.close();
}

void clientaccess::on_toolButton_fault_clear_clicked()
{
    int i=0;
 while(i<=ui->tableWidget_fault->rowCount())
 {
     ui->tableWidget_fault->removeRow(i);
     ++i;
 }
}

void clientaccess::on_toolButton_Fault_Return_clicked()
{
 ui->stackedWidget->setCurrentIndex(3);
 ui->label_Center->setText("Settings");
}

void clientaccess::on_toolButton_Trading_Export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Export Contact"), "",
        tr("text files(*.txt);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
//! [export function part1]

//! [export function part2]
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        return;
    }

   // QTextStream out(&file);
//! [export function part2]
     QTextStream stream(&file);
     QString IdTag=ui->lineEdit_Trading_CardId->text();
     QString Start_Time=ui->lineEdit_Trading_StartTime->text();
     QString Stop_Time=ui->lineEdit_Trading_StopTime->text();
     QString Charge_KWH=ui->lineEdit_Trading_ChargeKwh->text();
     QString Fare_KWH=ui->lineEdit_Trading_FareKwh->text();
     QString Total_Fare=ui->lineEdit_Trading_TotalFare->text();
     QString Service_Fare=ui->lineEdit_Trading_ServiceFare->text();
     stream << "-------------------------------------------------" << "\n\n"
            << "          Firstcall Automation Pvt Ltd           " << "\n\n"
            << "         ** Thank you and Come again **          " << "\n\n"
            << "-------------------------------------------------" << "\n\n";
     stream<<"\t"<<"Card_Id"<<"\t\t"<<IdTag<<"\n"<<"\t"<<"Start_Time"<<"\t"<<Start_Time<<"\n";
     stream<<"\t"<<"Stop_Time"<<"\t"<<Stop_Time<<"\n"<<"\t"<<"Charge_KWH"<<"\t"<<Charge_KWH<<"\t"<<"kwH"<<"\n";
     stream<<"\t"<<"Fare_KWH"<<"\t"<<Fare_KWH<<"\t"<<"Rs."<<"\n"<<"\t"<<"Total_Fare"<<"\t"<<Total_Fare<<"\t"<<"Rs."<<"\n";
     stream<<"\t"<<"Service_Fare"<<"\t"<<Service_Fare<<"\n";
     stream << "-------------------------------------------------";
     QString text="-------------------------------------------------\n\n"
             "          Firstcall Automation Pvt Ltd           \n\n"
             "         ** Thank you and Come again **          \n\n"
             "-------------------------------------------------\n\n"
             "\tCard_Id\t\t"+IdTag+"\n\tStart_Time\t"+Start_Time+"\n"
             "\tStop_Time\t"+Stop_Time+"\n\tCharge_KWH\t"+Charge_KWH+"\tkwH\n"
             "\tFare_KWH\t"+Fare_KWH+"\tRs.\n\tTotal_Fare\t"+Total_Fare+"\tRs.\n"
             "\tService_Fare\t"+Service_Fare+"\n-------------------------------------------------";

     ui->textEdit_Trading_bill->setText(text);
        QPrinter printer;

        QPrintDialog *dialog = new QPrintDialog(&printer,this);
        dialog->setWindowTitle("Print Document");

        if (dialog->exec() == QDialog::Rejected)return;
        ui->textEdit_Trading_bill->print(&printer);
     file.close();
}

void clientaccess::on_toolButton_IdTag_Cancel_clicked()
{
    ui->lineEdit_Idtag->clear();
    ui->stackedWidget->setCurrentIndex(13);
}

void clientaccess::on_toolButton_Confirm_IPConfig_r_clicked()
{
    QString ServeIP=ui->lineEdit_Server_IP_r->text();
    QString ServerPort=ui->lineEdit_Server_Port_r->text();
    QString MaCAddress=ui->lineEdit_MacAddress_r->text();
    QString Domain=ui->lineEdit_Domain_r->text();
    QString IPMode=ui->comboBox_IPMode_r->currentText();
    QString LocalIP,GatewayIPAddress,Netmask,DNS;
    if(IPMode=="Auto")
    {
        foreach(const QHostAddress &address,QNetworkInterface::allAddresses())
        {
            if(address.protocol()==QAbstractSocket::IPv4Protocol && address !=QHostAddress(QHostAddress::LocalHost))
            {
                  LocalIP= address.toString();
             }
         }
         foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
             foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
                 if (entry.ip().toString() == LocalIP) {
//                     localMacAddress = networkInterface.hardwareAddress();
                     Netmask = entry.netmask().toString();
                     break;
                 }
             }
         }
    }
    if(IPMode=="Manual")
    {
        LocalIP=ui->lineEdit_LocalIP_r->text();
        GatewayIPAddress=ui->lineEdit_GwIPAddr_r->text();
        Netmask=ui->lineEdit_NetMask_r->text();
        DNS=ui->lineEdit_DNS_r->text();
    }
w2->d.update_clientip(LocalIP,ClientId);
// Automatically Fetching IP from Localhost System
foreach(const QHostAddress &address,QNetworkInterface::allAddresses())
{
    if(address.protocol()==QAbstractSocket::IPv4Protocol && address !=QHostAddress(QHostAddress::LocalHost))
    {
        w2->w->ip=address.toString();
    }
}
w2->d.get_client_id(&w2->w->ClientId,w2->w->ip);
w2->d.get_connectorids(&w2->w->ConnectorId,w2->w->ip); // Get Connectorids from connectorid details table using clientid
w2->d.get_NoofConnectors_usingclienttable(&w2->w->No_of_Connectors,w2->w->ClientId);// Get Number of Connectors from data(client_table)
//     Split all Connectorids individually and stored in Array
QStringList Connector=w2->w->ConnectorId.split('/');
//No_of_Connectors=Connector.count();
for(int i=0;i<w2->w->No_of_Connectors;i++)
{
    w2->w->Connectors[i]=Connector.at(i+1);
}
w2->w->timer->start(0);
w2->d.get_interval(w2->w->ClientId,&w2->w->interval);
w2->w->timer2->start(w2->w->interval*3600*1000);
QString clientId;
QString managerId;
w2->w->d.get_clientid_using_IP(&clientId,LocalIP);
w2->w->d.get_managerid_using_clientid(&managerId,clientId);
ui->lineEdit_ClientID->setText(clientId);
ui->lineEdit_ManagerID->setText(managerId);
QString ConnectorIdfor7,ConnectorIdfor8,ConnectorIdfor1,ConnectorIdfor2,ConnectorIdfor3,ConnectorIdfor4,ConnectorIdfor5,ConnectorIdfor6;
w2->d.get_chargertype(ClientId,&ConnectorIdfor7,&ConnectorIdfor8,&ConnectorIdfor1,&ConnectorIdfor2,&ConnectorIdfor3,&ConnectorIdfor4,&ConnectorIdfor5,&ConnectorIdfor6);//get DC connectors
if(((ConnectorIdfor8!=0)||(ConnectorIdfor7!=0))&&((ConnectorIdfor1!=0)||(ConnectorIdfor2!=0)||(ConnectorIdfor3!=0)||(ConnectorIdfor4!=0)||(ConnectorIdfor5!=0)||(ConnectorIdfor6!=0)))
{
    ui->toolButton_AC_Rate_r->show();
    ui->toolButton_DC_Rate_r->show();
    ui->stackedWidget->setCurrentIndex(12);
    ui->stackedWidget_Registration_Login->setCurrentIndex(6);
}
else if((ConnectorIdfor8!=0)||(ConnectorIdfor7!=0))
{
    ui->toolButton_AC_Rate_r->setEnabled(false);
    ui->toolButton_AC_Rate_r->hide();
    ui->stackedWidget->setCurrentIndex(12);
    ui->stackedWidget_Registration_Login->setCurrentIndex(6);
    ui->stackedWidget_ACDCRate_2->setCurrentIndex(1);
}
else if((ConnectorIdfor1!=0)||(ConnectorIdfor2!=0)||(ConnectorIdfor3!=0)||(ConnectorIdfor4!=0)||(ConnectorIdfor5!=0)||(ConnectorIdfor6!=0))
{
    ui->toolButton_DC_Rate_r->setEnabled(false);
    ui->toolButton_DC_Rate_r->hide();
    ui->stackedWidget->setCurrentIndex(12);
    ui->stackedWidget_Registration_Login->setCurrentIndex(6);
    ui->stackedWidget_ACDCRate_2->setCurrentIndex(0);
}
}

void clientaccess::on_toolButton_Cancel_IPConfig_r_clicked()
{
    ui->lineEdit_login_ClienId->clear();
    ui->lineEdit_login_Password->clear();
    ui->stackedWidget_Registration_Login->setCurrentIndex(3);
}

void clientaccess::on_toolButton_DC_Enter_r_clicked()
{
    QString DC_00=ui->lineEdit_DC_00_r->text();
    QString DC_01=ui->lineEdit_DC_01_r->text();
    RatePeriod1_Start=DC_00+":"+DC_01+":00";
    QString DC_02=ui->lineEdit_DC_02_r->text();
    QString DC_03=ui->lineEdit_DC_03_r->text();
    RatePeriod1_Stop=DC_02+":"+DC_03+":00";
    Rate1=ui->lineEdit_DC_04_r->text();
    QString DC_10=ui->lineEdit_DC_10_r->text();
    QString DC_11=ui->lineEdit_DC_11_r->text();
    RatePeriod2_Start=DC_10+":"+DC_11+":00";
    QString DC_12=ui->lineEdit_DC_12_r->text();
    QString DC_13=ui->lineEdit_DC_13_r->text();
    RatePeriod2_Stop=DC_12+":"+DC_13+":00";
    Rate2=ui->lineEdit_DC_14_r->text();
    QString DC_20=ui->lineEdit_DC_20_r->text();
    QString DC_21=ui->lineEdit_DC_21_r->text();
    RatePeriod3_Start=DC_20+":"+DC_21+":00";
    QString DC_22=ui->lineEdit_DC_22_r->text();
    QString DC_23=ui->lineEdit_DC_23_r->text();
    RatePeriod3_Stop=DC_22+":"+DC_23+":00";
    Rate3=ui->lineEdit_DC_24_r->text();
    QString DC_30=ui->lineEdit_DC_30_r->text();
    QString DC_31=ui->lineEdit_DC_31_r->text();
    RatePeriod4_Start=DC_30+":"+DC_31+":00";
    QString DC_32=ui->lineEdit_DC_32_r->text();
    QString DC_33=ui->lineEdit_DC_33_r->text();
    RatePeriod4_Stop=DC_32+":"+DC_33+":00";
    Rate4=ui->lineEdit_DC_34_r->text();
    QDateTime issueDate = QDateTime::currentDateTime();
    QString CurrentDate=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    w2->d.insert_Rate_Settings(ClientId,"DC",RatePeriod1_Start,RatePeriod1_Stop,RatePeriod2_Start,RatePeriod2_Stop,RatePeriod3_Start,RatePeriod3_Stop,RatePeriod4_Start,RatePeriod4_Stop,Rate1,Rate2,Rate3,Rate4,CurrentDate);
    ui->stackedWidget->setCurrentIndex(13);
}

void clientaccess::on_toolButton_AC_Enter_r_clicked()
{
    QString AC_00=ui->lineEdit_AC_00_r->text();
    QString AC_01=ui->lineEdit_AC_01_r->text();
    RatePeriod1_Start=AC_00+":"+AC_01+":00";
    QString AC_02=ui->lineEdit_AC_02_r->text();
    QString AC_03=ui->lineEdit_AC_03_r->text();
    RatePeriod1_Stop=AC_02+":"+AC_03+":00";
    Rate1=ui->lineEdit_AC_04_r->text();
    QString AC_10=ui->lineEdit_AC_10_r->text();
    QString AC_11=ui->lineEdit_AC_11_r->text();
    RatePeriod2_Start=AC_10+":"+AC_11+":00";
    QString AC_12=ui->lineEdit_AC_12_r->text();
    QString AC_13=ui->lineEdit_AC_13_r->text();
    RatePeriod2_Stop=AC_12+":"+AC_13+":00";
    Rate2=ui->lineEdit_AC_14_r->text();
    QString AC_20=ui->lineEdit_AC_20_r->text();
    QString AC_21=ui->lineEdit_AC_21_r->text();
    RatePeriod3_Start=AC_20+":"+AC_21+":00";
    QString AC_22=ui->lineEdit_AC_22_r->text();
    QString AC_23=ui->lineEdit_AC_23_r->text();
    RatePeriod3_Stop=AC_22+":"+AC_23+":00";
    Rate3=ui->lineEdit_AC_24_r->text();
    QString AC_30=ui->lineEdit_AC_30_r->text();
    QString AC_31=ui->lineEdit_AC_31_r->text();
    RatePeriod4_Start=AC_30+":"+AC_31+":00";
    QString AC_32=ui->lineEdit_AC_32_r->text();
    QString AC_33=ui->lineEdit_AC_33_r->text();
    RatePeriod4_Stop=AC_32+":"+AC_33+":00";
    Rate4=ui->lineEdit_AC_34_r->text();
    QDateTime issueDate = QDateTime::currentDateTime();
    QString CurrentDate=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    w2->w->d.insert_Rate_Settings(ClientId,"AC",RatePeriod1_Start,RatePeriod1_Stop,RatePeriod2_Start,RatePeriod2_Stop,RatePeriod3_Start,RatePeriod3_Stop,RatePeriod4_Start,RatePeriod4_Stop,Rate1,Rate2,Rate3,Rate4,CurrentDate);
    ui->stackedWidget->setCurrentIndex(13);
}

void clientaccess::on_toolButton_return_commonPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
}

void clientaccess::on_toolButton_Keyboard_clicked()
{
    QProcess *process = new QProcess(this);
    QString file = "C:/Windows/System32/osk.exe";
    process->start(file);
}

void clientaccess::on_toolButton_exit_clicked()
{
    showMinimized();
    QProcess::startDetached("taskkill /F /IM ""osk.exe"" /T");
}

void clientaccess::on_toolButton_Trading_Clear_clicked()
{
    ui->lineEdit_Trading_CardId->clear();
    ui->lineEdit_Trading_StartTime->clear();
    ui->lineEdit_Trading_StopTime->clear();
    ui->lineEdit_Trading_ChargeKwh->clear();
    ui->lineEdit_Trading_FareKwh->clear();
    ui->lineEdit_Trading_TotalFare->clear();
    ui->lineEdit_Trading_ServiceFare->clear();
}

void clientaccess::on_toolButton_QRScan_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void clientaccess::on_toolButton_RFID_Scan_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void clientaccess::on_toolButton_QR_Confirm_clicked()
{
QString  UserName=ui->lineEdit_QR_Name->text();
QString  OTP=ui->lineEdit_QR_OTP->text();
QString QR_IdTag;
w2->d.get_idtag_QR(OTP,UserName,&QR_IdTag);
w2->w->IdTag=QR_IdTag;
QMessageBox::information(this,tr("Info IDTAG"),UserName+OTP+QR_IdTag);
w2->w->on_pushButton_Send_Authorization_Req_clicked();
    if((UserName!="")&&(OTP !="")&&(w2->w->IdTag!=""))
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->lineEdit_QR_Name->clear();
        ui->lineEdit_QR_OTP->clear();
        w2->d.delete_OTP_QR(UserName,w2->w->IdTag);
    }
    else
    {
        QMessageBox::warning(this,tr("INFO"),tr("Please Insert a Valid OTP"));
    }
    UserName="";OTP="";
}

void clientaccess::on_toolButton_QR_Cancel_clicked()
{
    ui->lineEdit_QR_Name->clear();
    ui->lineEdit_QR_OTP->clear();
    ui->stackedWidget->setCurrentIndex(13);
}

void clientaccess::on_toolButton_register_submit_manage_clicked()
{
    QString MeterSerialNo=ui->lineEdit_MeterSerialNo_manage->text();
    QString ManagerName=ui->lineEdit_ClientName_manage->text();
    QString CompanyName=ui->lineEdit_CompanyName_manage->text();
    QString ManagerMailId=ui->lineEdit_MailId_manage->text();
    QString ManagerAddress=ui->lineEdit_Address_manage->text();
    QString ManagerMobileNo=ui->lineEdit_MobileNo_manage->text();
    QString ManagerLandLineNo=ui->lineEdit_LandlineNo_manage->text();
    QString ManagerPassword=ui->lineEdit_Password_manage->text();
    QString ReEnter_Password=ui->lineEdit_ReenterPassword_manage->text();
    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    QRegExp mobileREX("\\d{10}");
    QRegExp landlineREX("\\d{10}");
    w2->w->d.get_clientidforregistration(&ClientId,MeterSerialNo);
    if(ClientId==NULL)
    {
        ui->lineEdit_MeterSerialNo->clear();
        QMessageBox::information(this,tr("Info"),tr("Please enter valid Meter Serial Number"));
    }
    else
    {
        if(mobileREX.exactMatch(ManagerMobileNo)==false)
        {
            ui->lineEdit_MobileNo->clear();
            QMessageBox::information(this,tr("Info"),tr("Please re-enter the Mobile Number"));
        }
        else
        {
            if(landlineREX.exactMatch(ManagerLandLineNo)==false)
            {
                ui->lineEdit_LandlineNo->clear();
                QMessageBox::information(this,tr("Info"),tr("Please re-enter the LandLine Number \n LandLine Format = pincodeYourNo"));
            }
            else
            {
                if(mailREX.exactMatch(ManagerMailId)==false)
                {
                    ui->lineEdit_MailId->clear();
                    QMessageBox::information(this,tr("Info"),tr("Please re-enter the MailId"));
                }
                else
                {
                    int passwordLen = ui->lineEdit_Password->text().length();
                    if(passwordLen < 8)
                    {
                        ui->label_passwordstre->setText("Your password must be at least 8 characters\n");
                    }
                    else
                    {
                        ui->label_passwordstre->setText("Your password is good");
                        if(ManagerPassword==ReEnter_Password)
                        {
                            quint16 Transaction= QRandomGenerator::global()->generate();
                            QString Id = QString::number(Transaction);
                            QString ManagerId=ManagerName+Id;
                            w2->w->d.manager_registration_insert(ClientId,ManagerId,ManagerName,CompanyName,ManagerMailId,ManagerAddress,ManagerMobileNo,ManagerLandLineNo,ManagerPassword);
                            ui->stackedWidget_Registration_Login->setCurrentIndex(3);
                            ui->lineEdit_login_ClienId->setText(ClientId);

                        }
                        else
                        {
                            ui->lineEdit_ReenterPassword->clear();
                            QMessageBox::information(this,tr("Info"),tr("Please re-enter the Password"));
                        }
                    }
                }
            }
        }
    }
}

void clientaccess::on_toolButton_register_cancel_manage_clicked()
{
    ui->stackedWidget_Registration_Login->setCurrentIndex(0);
    ui->lineEdit_MeterSerialNo_manage->clear();
    ui->lineEdit_ClientName_manage->clear();
    ui->lineEdit_CompanyName_manage->clear();
    ui->lineEdit_MailId_manage->clear();
    ui->lineEdit_Address_manage->clear();
    ui->lineEdit_MobileNo_manage->clear();
    ui->lineEdit_LandlineNo_manage->clear();
    ui->lineEdit_Password_manage->clear();
    ui->lineEdit_ReenterPassword_manage->clear();
}
void clientaccess::on_toolButton_SystemSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_Center->setText("System Settings");
    ui->toolButton_Settings->show();
    ui->toolButton_Info->show();
}

void clientaccess::on_toolButton_Costset_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    QString ConnectorIdfor7,ConnectorIdfor8,ConnectorIdfor1,ConnectorIdfor2,ConnectorIdfor3,ConnectorIdfor4,ConnectorIdfor5,ConnectorIdfor6;
    w2->d.get_chargertype(ClientId,&ConnectorIdfor7,&ConnectorIdfor8,&ConnectorIdfor1,&ConnectorIdfor2,&ConnectorIdfor3,&ConnectorIdfor4,&ConnectorIdfor5,&ConnectorIdfor6);//get DC connectors
    if((ConnectorIdfor8!=0)||(ConnectorIdfor7!=0))
    {
        ui->toolButton_AC_Rate->setEnabled(false);
        ui->toolButton_AC_Rate->hide();
        ui->stackedWidget->setCurrentIndex(12);
        ui->stackedWidget_Registration_Login->setCurrentIndex(6);
        ui->stackedWidget_ACDCRate->setCurrentIndex(1);
    }
    else if((ConnectorIdfor1!=0)||(ConnectorIdfor2!=0)||(ConnectorIdfor3!=0)||(ConnectorIdfor4!=0)||(ConnectorIdfor5!=0)||(ConnectorIdfor6!=0))
    {
        ui->toolButton_DC_Rate->setEnabled(false);
        ui->toolButton_DC_Rate->hide();
        ui->stackedWidget->setCurrentIndex(12);
        ui->stackedWidget_Registration_Login->setCurrentIndex(6);
        ui->stackedWidget_ACDCRate->setCurrentIndex(0);
    }
    else
    {
        ui->toolButton_AC_Rate->show();
        ui->toolButton_DC_Rate->show();
        ui->stackedWidget->setCurrentIndex(12);
        ui->stackedWidget_Registration_Login->setCurrentIndex(6);
    }
    ui->label_Center->setText("Cost Set");
    ui->toolButton_Settings->show();
    ui->toolButton_Info->show();
}

void clientaccess::on_toolButton_IPSet_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->stackedWidget_Registration_Login->setCurrentIndex(2);
    ui->label_Center->setText("IP Set");
    ui->toolButton_Settings->show();
    ui->toolButton_Info->show();
}

void clientaccess::on_toolButton_OtherSet_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->label_Center->setText("Other Set");
    ui->toolButton_Settings->show();
    ui->toolButton_Info->show();
}

void clientaccess::on_toolButton_fault_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->label_Center->setText("Fault");
    ui->toolButton_Settings->show();
    ui->toolButton_Info->show();
}

void clientaccess::on_toolButton_Trading_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
    ui->label_Center->setText("Trading");
    ui->toolButton_Settings->show();
    ui->toolButton_Info->show();
}

void clientaccess::on_toolButton_Manage_clicked()
{
 ui->stackedWidget->setCurrentIndex(10);
 ui->label_Center->setText("Manage");
 ui->toolButton_Settings->show();
 ui->toolButton_Info->hide();
 ui->tableWidget_ChargerInfo_Manage->setRowCount(5);
 w2->d.get_NoofConnectors_usingclienttable(&w2->w->No_of_Connectors,w2->w->ClientId);
 ui->tableWidget_ChargerInfo_Manage->setColumnCount(w2->w->No_of_Connectors);
 w2->d.get_connectorids(&w2->w->ConnectorId,w2->w->ip); // Get Connectorids from connectorid details table using clientid
 QStringList Connector=w2->w->ConnectorId.split('/');
 for(int i=0;i<w2->w->No_of_Connectors;i++)
 {QString ChargerType,ChargerPower,ChargerCurrent,SocketType;
     w2->w->Connectors[i]=Connector.at(i+1);
     ui->tableWidget_ChargerInfo_Manage->setItem(0,i,new QTableWidgetItem(w2->w->Connectors[i]));
     w2->d.get_chargertype_using_connectorid(&ChargerType,&ChargerPower,&ChargerCurrent,&SocketType,w2->w->Connectors[i]);
     ui->tableWidget_ChargerInfo_Manage->setItem(1,i,new QTableWidgetItem(ChargerType));
     ui->tableWidget_ChargerInfo_Manage->setItem(2,i,new QTableWidgetItem(ChargerPower));
     ui->tableWidget_ChargerInfo_Manage->setItem(3,i,new QTableWidgetItem(ChargerCurrent));
     ui->tableWidget_ChargerInfo_Manage->setItem(4,i,new QTableWidgetItem(SocketType));
 }
}
void clientaccess::on_toolButton_Settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(14);
    ui->toolButton_SystemSettings->hide();
    ui->toolButton_Costset->hide();
    ui->toolButton_IPSet->hide();
    ui->toolButton_OtherSet->hide();
    ui->toolButton_fault->hide();
    ui->toolButton_Trading->hide();
    ui->toolButton_Manage->hide();
    ui->toolButton_Info->hide();
    ui->label_Center->setText("Login");
    ui->toolButton_Info->show();
}


void clientaccess::on_toolButton_Info_clicked()
{
    ui->label_Center->setText("Device Information");
    ui->stackedWidget->setCurrentIndex(11);
    ui->toolButton_Info->show();
    QPixmap pix(":/icons/charging-station.png");
    int DeviceInfoW = ui->label_DeviceInfopic->width();
    int DeviceInfoH = ui->label_DeviceInfopic->height();
    ui->label_DeviceInfopic->setPixmap(pix.scaled(DeviceInfoW,DeviceInfoH,Qt::KeepAspectRatio));

}

void clientaccess::on_toolButton_Client_Login_Settings_clicked()
{
    QString Id=ui->lineEdit_login_ClienId_Settings->text();
    QString Password = ui->lineEdit_login_Password_Settings->text();
    QString password_client,password_manager;
    w2->w->d.get_passwordforlogin(&password_client,Id);
    w2->w->d.get_passwordformanagerlogin(&password_manager,Id);
    if((password_client==Password)&&(Password!=NULL)&&(Id!=NULL))
    {QString credentials,credential_pages[8];
        w2->d.get_logincredentials(&credentials,"client");
        qDebug()<<"credentials"<<credentials;
        QStringList lines = credentials.split('+');
        int numRows = lines.count();
        for(int i=0;i<numRows;i++)
        {
            credential_pages[i]=lines.at(i);
        }
        for(int i=0;i<numRows;i++)
        {
        if(credential_pages[i]=="1")
        {
            ui->toolButton_SystemSettings->show();
        }
        if(credential_pages[i]=="2")
        {
            ui->toolButton_Costset->show();
        }
        if(credential_pages[i]=="3")
        {
            ui->toolButton_IPSet->show();
        }
        if(credential_pages[i]=="4")
        {
            ui->toolButton_OtherSet->show();
        }
        if(credential_pages[i]=="5")
        {
            ui->toolButton_fault->show();
        }
        if(credential_pages[i]=="6")
        {
            ui->toolButton_Trading->show();
        }
        if(credential_pages[i]=="7")
        {
            ui->toolButton_Manage->show();
        }
        if(credential_pages[i]=="8")
        {
            ui->toolButton_Info->show();
        }
        }
        ui->stackedWidget->setCurrentIndex(3);
        ui->lineEdit_login_ClienId_Settings->clear();
        ui->lineEdit_login_Password_Settings->clear();
    }
    else if((password_manager==Password)&&(Password!=NULL)&&(Id!=NULL))
    {
        QString credentials,credential_pages[8];
        w2->d.get_logincredentials(&credentials,"manager");
        qDebug()<<"credentials"<<credentials;
        QStringList lines = credentials.split('+');
        int numRows = lines.count();
        for(int i=0;i<numRows;i++)
        {
            credential_pages[i]=lines.at(i);
        }
        for(int i=0;i<numRows;i++)
        {
        if(credential_pages[i]=="1")
        {
            ui->toolButton_SystemSettings->show();
        }
        if(credential_pages[i]=="2")
        {
            ui->toolButton_Costset->show();
        }
        if(credential_pages[i]=="3")
        {
            ui->toolButton_IPSet->show();
        }
        if(credential_pages[i]=="4")
        {
            ui->toolButton_OtherSet->show();
        }
        if(credential_pages[i]=="5")
        {
            ui->toolButton_fault->show();
        }
        if(credential_pages[i]=="6")
        {
            ui->toolButton_Trading->show();
        }
        if(credential_pages[i]=="7")
        {
            ui->toolButton_Manage->show();
        }
        if(credential_pages[i]=="8")
        {
            ui->toolButton_Info->show();
        }
        }
        ui->stackedWidget->setCurrentIndex(3);
        ui->lineEdit_login_ClienId_Settings->clear();
        ui->lineEdit_login_Password_Settings->clear();
    }
    else
    {
        QMessageBox::information(this,tr("Info"),tr("Please Enter Correct Password"));
        ui->lineEdit_login_Password_Settings->clear();
    }
}

void clientaccess::on_toolButton_Client_Login_Cancel_Settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(13);
    ui->lineEdit_login_ClienId_Settings->clear();
    ui->lineEdit_login_Password_Settings->clear();
}

void clientaccess::on_toolButton_info_bottom_clicked()
{
    on_toolButton_Info_clicked();
}

void clientaccess::on_toolButton_AC_Rate_r_clicked()
{
    ui->stackedWidget_ACDCRate_2->setCurrentIndex(0);
}

void clientaccess::on_toolButton_DC_Rate_r_clicked()
{
    ui->stackedWidget_ACDCRate_2->setCurrentIndex(1);
}
