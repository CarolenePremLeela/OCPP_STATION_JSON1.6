#include "mainwindow.h"
#include "ui_mainwindow.h"
QStringList lines ;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    auth->WriteData();
    //    auth->WriteDatadiag();
    //    r->WriteData();
    //    t->WriteData();
    ui->pushButton_StopChargePoint->setEnabled(false);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(func()));
    timer2=new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(heartbeatloop()));
    // Timer Start
ui->pushButton_StartChargePoint->animateClick(100);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::heartbeatloop()
{
    HeartBeat();
}
void MainWindow::func()
{
   QDateTime issueDate = QDateTime::currentDateTime();
   CurrentDate=issueDate.toString("yyyy-MM-dd HH:mm:ss"); // Convert currenttime into UTC time
    gmt=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    if(networkresult==true)
        {
        ui->statusBar->setStyleSheet("color:green");
            ui->statusBar->showMessage("ONLINE");
//            QLabel *test= new QLabel;
//            test->setPixmap(QPixmap(":/icons/disconnected.png"));
//            test->setAlignment(Qt::AlignRight);
//            ui->statusBar->addWidget(test);
            if(networkflag==true)
            {
                auth->update_list_diag(CurrentDate,"Connected");
                networkflag=false;
            }
        }
    else if(networkresult==false)
        {
        ui->statusBar->setStyleSheet("color:red");
            ui->statusBar->showMessage("OFFLINE");
//            QLabel *test1= new QLabel;
//            test1->setPixmap(QPixmap(":/icons/unavailable.ico"));
//            test1->setAlignment(Qt::AlignLeft);
//            ui->statusBar->addWidget(test1);
            if(networkflag==false)
            {
                auth->update_list_diag(CurrentDate,"Disconnected");
                networkflag=true;
            }
        }
    QString time=issueDate.toString("dd-MM-yyyy hh:mm:ss");
    ui->lcdNumber->display(time); // Displaying current time in lcd
    if(networkresult==true)
    {
    // Get Status of trigger message from bitset table using client id
    d.get_bitset(&boot,&diag,&firm,&heart,&statusnoti,client_id);
    for(int i=0;i<No_of_Connectors;i++)
    {
        // Get change availability status from Connector id details table using ConnectorId
        d.get_client_changeavailability(&avail[i],Connectors[i]);
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
         // Get Remote Start status from Connector id details table using ConnectorId
        d.get_client_remotestartstatus(&start[i],Connectors[i]);
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
        // Get Remote Stop status from Connector id details table using ConnectorId
        d.get_client_remotestopstatus(&stop[i],Connectors[i]);
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
        // Get Metervalues Status for trigger message from Connectorid details table using connectorid
        d.get_client_triggermetervalue(&mtrval[i],Connectors[i]);
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
        // Get Meterstate from connectorid details table using connectorid
        d.get_client_meter_ONOFF_status(&mON[i],Connectors[i]);
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
        // Get Reserve Status from Connectordetails table using connectorid
        d.reserveNow_Status(&res[i],Connectors[i]);
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
       if(start[i]==1 ) // Which connectors remote start status is 1 that connector id is passed to remote start transaction fucntion
       {
           ConnectorId=Connectors[i];
           RemoteStartTransaction();
       }
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
       if(stop[i]== 1 ) // Which connectors remote stop status is 1 that connector id is passed to remote stop transaction fucntion
       {
           ConnectorId=Connectors[i];
           RemoteStopTransaction();
       }
    }
    for(int i=0;i<No_of_Connectors;i++)
    {
        if(mtrval[i]==1) // Which connectors Metervalue status is 1 that connector id is passed to Metervalue fucntion
        {
        ConnectorId=Connectors[i];
        MeterValues();
        }
    }
    if(boot==1) // If Boot is equal to one then go to Bootnotification loop
    {
        BootNotification();
    }
    if(diag==1) // If diag is equal to one then go to DiagnosticsStatusNotification loop
    {
        DiagnosticsStatusNotification();
    }
    if(firm==1) // If firm is equal to one then go to FirmwareStatusNotification loop
    {
        FirmwareStatusNotification();
    }
    if(heart==1) // If heart is equal to one then go to HeartBeat loop
    {
        HeartBeat();
    }
    if(statusnoti==1) // If statusnoti is equal to one then go to StatusNotification loop
    {
        StatusNotification();
    }

    if(res[0]==1)
    {
        // If Connector1 is reserved then Green Indication
        ui->frame_con1->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector1 is Unreserved then Red Indication
        ui->frame_con1->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(res[1]==1)
    {
         // If Connector2 is reserved then Green Indication
        ui->frame_con2->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector2 is Unreserved then Red Indication
        ui->frame_con2->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(res[2]==1)
    {
         // If Connector3 is reserved then Green Indication
        ui->frame_con3->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector3 is Unreserved then Red Indication
        ui->frame_con3->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(res[3]==1)
    {
        // If Connector4 is reserved then Green Indication
        ui->frame_con4->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector4 is Unreserved then Red Indication
        ui->frame_con4->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(res[4]==1)
    {
        // If Connector5 is reserved then Green Indication
        ui->frame_con5->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector5 is Unreserved then Red Indication
        ui->frame_con5->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(res[5]==1)
    {
        // If Connector6 is reserved then Green Indication
        ui->frame_con6->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector6 is Unreserved then Red Indication
        ui->frame_con6->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(res[6]==1)
    {
        // If Connector7 is reserved then Green Indication
        ui->frame_con7->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector7 is Unreserved then Red Indication
        ui->frame_con7->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(res[7]==1)
    {
        // If Connector8 is reserved then Green Indication
        ui->frame_con8->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
    }
    else
    {
        // If Connector8 is Unreserved then Red Indication
        ui->frame_con8->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
    }
    if(avail[0]==0)
    {
        // If Connector1 is not available - Black Indication
        ui->frame_11->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[0]==1)
        {
            // If Connector1 available and Meter is ON - Green Indication
            ui->frame_11->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector1 available and Meter is OFF - Red Indication
            ui->frame_11->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    if(avail[1]==0)
    {
        // If Connector2 is not available - Black Indication
        ui->frame_22->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[1]==1)
        {
            // If Connector2 available and Meter is ON - Green Indication
            ui->frame_22->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector2 available and Meter is OFF - Red Indication
            ui->frame_22->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    if(avail[2]==0)
    {
        // If Connector3 is not available - Black Indication
        ui->frame_33->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[2]==1)
        {
            // If Connector3 available and Meter is ON - Green Indication
            ui->frame_33->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector3 available and Meter is OFF - Red Indication
            ui->frame_33->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    if(avail[3]==0)
    {
        // If Connector4 is not available - Black Indication
        ui->frame_44->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[3]==1)
        {
            // If Connector4 available and Meter is ON - Green Indication
            ui->frame_44->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector4 available and Meter is OFF - Red Indication
            ui->frame_44->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    if(avail[4]==0)
    {
        // If Connector5 is not available - Black Indication
        ui->frame_55->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[4]==1)
        {
            // If Connector5 available and Meter is ON - Green Indication
            ui->frame_55->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector5 available and Meter is OFF - Red Indication
            ui->frame_55->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    if(avail[5]==0)
    {
        // If Connector6 is not available - Black Indication
        ui->frame_66->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[5]==1)
        {
            // If Connector6 available and Meter is ON - Green Indication
            ui->frame_66->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector6 available and Meter is OFF - Red Indication
            ui->frame_66->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    if(avail[6]==0)
    {
        // If Connector7 is not available - Black Indication
        ui->frame_77->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[6]==1)
        {
            // If Connector7 available and Meter is ON - Green Indication
            ui->frame_77->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector7 available and Meter is OFF - Red Indication
            ui->frame_77->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    if(avail[7]==0)
    {
        // If Connector8 is not available - Black Indication
        ui->frame_88->setStyleSheet("QFrame{background-color:rgb(0, 0, 0); border-radius:20px;}");
    }
    else
    {
        if(mON[7]==1)
        {
            // If Connector8 available and Meter is ON - Green Indication
            ui->frame_88->setStyleSheet("QFrame{background-color:green; border-radius:20px;}");
        }
        else
        {
            // If Connector8 available and Meter is OFF - Red Indication
            ui->frame_88->setStyleSheet("QFrame{background-color:red; border-radius:20px;}");
        }
    }
    }
}
void MainWindow::on_pushButton_StartChargePoint_clicked()
{if(networkresult==true)
    {
   // Start charge point button Disabled
  ui->pushButton_StartChargePoint->setEnabled(false);

  // Stop Charge point button Enabled
  ui->pushButton_StopChargePoint->setEnabled(true);

  ui->frame_2->setStyleSheet("background-color:rgb(146, 255, 150)");
  ui->frametext->setText("The ChargePoint is running");


  /*r->get_no_of_connectors("client_id",&ClientId); //Get clientid from configuration list
  client_id=ClientId.toInt();
  r->get_no_of_connectors("Number_of_Connectors",&number); // Get Number of connectors from configuration list
  No_of_Connectors=number.toInt();
  qDebug()<<"No_of_Connectors"<<No_of_Connectors;
  // Get Connectorids from configuration list
  for(int i=1;i<=No_of_Connectors;i++)
  {
  QString s = QString::number(i);
  r->get_no_of_connectors("Connector"+s,&Connectors[(i-1)+(client_id*10)]);
  }*/
  QString ipserver=ui->lineEdit_ip->text();
  QString port=ui->lineEdit_port->text(); // Assigning Port Number for listerning
  QString Url="ws://"+ipserver+":"+port+"/ocppj16/ChargePoint"+ClientId;
  qDebug() << "WebSocket server:" << Url; // Websocket Url to connect to CENTRAL SYSTEM server
  connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);// If text Message received from socket, go to Received loop
//  connect(&m_webSocket, &QWebSocket::disconnected, this, &MainWindow::closed); // If Websocket disconnected, go to closed1 loop
  m_webSocket.open(QUrl(Url)); // Open Websocket
    }
}
void MainWindow::closed()
{if(networkresult==true)
    {
    // If Server is offline then client window is restarting till server is connecting
//    qApp->quit();
//    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        on_pushButton_StartChargePoint_clicked();
    }
}
void MainWindow::on_pushButton_StopChargePoint_clicked()
{if(networkresult==true)
    {
    ui->pushButton_StartChargePoint->setEnabled(true);
    ui->pushButton_StopChargePoint->setEnabled(false);
    ui->frame_2->setStyleSheet("background-color:rgb(255, 76, 60)");
    ui->frametext->setText("The ChargePoint was stopped");
    m_webSocket.close(); //  Close websocket
    }
}
void MainWindow::Received(QString message)
{if(networkresult==true)
    {
    if(message!=NULL)
    {
    qDebug() << "message Recieved" <<message;
    lines = message.split(',');
    msg_Id =lines.at(0);
    uniq_Id=lines.at(1);
    PayloadHead =lines.at(2);
    Payload=lines.at(3);
    if(msg_Id=="3")
    {
        Sub3();
    }
    }
    }
}
void MainWindow::Sub3()
{
    if(PayloadHead=="Authorize")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
         QString y=lines.at(1);
         QString u=lines.at(2);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = y.split('=');
        QString y2=lines.at(1);
        lines = u.split('}');
        u=lines.at(0);
        lines = u.split('=');
        QString u2=lines.at(1);
        if(x1=="Status")
        {
            Status.append(x2);
            ExpiryDate.append(y2);
            ParentIdTag.append(u2);
            ui->lineEdit_Status_Authorize_Response->setText(Status);
            ui->lineEdit_ExpiryDate_Authorize_Response->setText(ExpiryDate);
            ui->lineEdit_ParentIdTag_Authorize_Response->setText(ParentIdTag);
            auth->on_pushButton_clicked();
            auth->update_list(IdTag,ExpiryDate,ParentIdTag);
        }
        ParentIdTag="";ExpiryDate="";
        if(Status!="Accepted")
        {
            IdTag="";
            Authorizevalue=0;
        }else Authorizevalue=1;
        Status="";
    }
    if(PayloadHead=="BootNotification")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
         QString y=lines.at(1);
         QString u=lines.at(2);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = y.split('=');
        QString y2=lines.at(1);
        lines = u.split('}');
        u=lines.at(0);
        lines = u.split('=');
        QString u2=lines.at(1);
        if(x1=="Interval")
        {
            Interval.append(x2);
            CurrentTime.append(y2);
            Status.append(u2);
            ui->lineEdit_Interval_bootnotification_Response->setText(Interval);
            interval=Interval.toInt();
            ui->lineEdit_CurrentTime_bootnotification_Response->setText(CurrentTime);
            ui->lineEdit_Status_bootnotification_Response->setText(Status);
            Status="";CurrentTime="";Interval="";
        }
    }
    if(PayloadHead=="DataTransfer")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
         QString y=lines.at(1);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = y.split('=');
        QString y2=lines.at(1);
        lines = y.split('=');
        y2=lines.at(1);
        if(x1=="Interval")
        {
            Data.append(x2);
            Status.append(y2);
            ui->lineEdit_DataReceived_Datatransfer_Response->setText(Data);
            ui->lineEdit_Status_Datatransfer_Response->setText(Status);
            Status="";Data="";
        }
    }
    if(PayloadHead=="DiagnosticsStatusNotification")
    {
        lines = Payload.split('}');
        QString u=lines.at(0);
        lines=u.split('{');
        u=lines.at(1);
        if(u=="Acknowledgementfromserver")
        {
        }
    }
    if(PayloadHead=="FirmwareStatusNotification")
    {
        lines = Payload.split('}');
        QString u=lines.at(0);
        lines=u.split('{');
        u=lines.at(1);
        if(u=="Acknowledgementfromserver")
        {
        }
    }
    if(PayloadHead=="HeartBeat")
    {
        lines = Payload.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines =x2.split('}');
        x2=lines.at(0);
        if(x1=="CurrentTime")
        {
            CurrentTime.append(x2);
            ui->lineEdit_CurrentTime_HeartBeat_Response->setText(CurrentTime);
            CurrentTime="";
        }
    }
    if(PayloadHead=="StartTransaction")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
         QString y=lines.at(1);
         QString u=lines.at(2);
         QString v=lines.at(3);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = y.split('=');
        QString y2=lines.at(1);
        lines = u.split('=');
        QString u2=lines.at(1);
        lines = v.split('=');
        QString v2=lines.at(1);
        lines =v2.split('}');
        v2=lines.at(0);
        if(x1=="ExpiryDate")
        {
            ExpiryDate.append(x2);
            ParentIdTag.append(y2);
            Status.append(u2);
            TransactionId.append(v2);
            ui->lineEdit_Expirydate_Start_Transaction_Response->setText(ExpiryDate);
            ui->lineEdit_parentID_Start_Transaction_Response->setText(ParentIdTag);
            ui->lineEdit_Status_Start_Transaction_Response->setText(Status);
            ui->lineEdit_TransactionID_Start_Transaction_Response->setText(TransactionId);
            ExpiryDate="";ParentIdTag="";Status="";
            MeterValues();
        }
    }
    if(PayloadHead=="StopTransaction")
    {
        lines = Payload.split('|');
         QString x=lines.at(0);
         QString y=lines.at(1);
         QString u=lines.at(2);
        lines = x.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        lines=x1.split('{');
        x1=lines.at(1);
        lines = y.split('=');
        QString y2=lines.at(1);
        lines = u.split('=');
        QString u2=lines.at(1);
        lines =u2.split('}');
        u2=lines.at(0);
        if(x1=="ExpiryDate")
        {
            ExpiryDate.append(x2);
            ParentIdTag.append(y2);
            Status.append(u2);
            ui->lineEdit_Expirydate_StopTransaction_Resp->setText(ExpiryDate);
            ui->lineEdit_ParentID_StopTransaction_Resp->setText(ParentIdTag);
            ui->lineEdit_Status_StopTransaction_Resp->setText(Status);
            ExpiryDate="";ParentIdTag="";Status="";
        }
    }
    if(PayloadHead=="StatusNotification")
    {
        lines = Payload.split('}');
        QString u=lines.at(0);
        lines=u.split('{');
        u=lines.at(1);
        if(u=="Acknowledgementfromserver")
        {
            qDebug()<<u;
        }
    }
    if(PayloadHead=="MeterValue")
    {
        lines = Payload.split('}');
        QString u=lines.at(0);
        lines=u.split('{');
        u=lines.at(1);
        if(u=="Acknowledgementfromserver")
        {
           qDebug()<<u;
        }
    }
}
void MainWindow::Authorize()
{
//    IdTag=ui->lineEdit_IdTag_Authorize_Request->text();
    QString Authorize="2,"+ip+",Authorize,{IdTag="+IdTag+"}";
    m_webSocket.sendTextMessage(Authorize);
    qDebug()<<"Message sent :"<<Authorize;
    Authorize="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
}
void MainWindow::BootNotification()
{
    SerialNumber=ui->lineEdit_SerialNumber_bootnotification_Request->text();
    Model=ui->lineEdit_Model_bootnotification_Request->text();
    ChargePointSerialNumber=ui->lineEdit_SerialNumChgpoint_bootnotification_Request->text();
    Vendor=ui->lineEdit_vendor_bootnotification_Request->text();
    FirmwareVersion=ui->lineEdit_firmwareversion_bootnotification_Request->text();
    ICCID=ui->lineEdit_Iccid_bootnotification_Request->text();
    IMSI=ui->lineEdit_Imsi_bootnotification_Request->text();
    MeterSerialNumber=ui->lineEdit_MeterSerialNum_bootnotification_Request->text();
    MeterType=ui->lineEdit_MeterType_bootnotification_Request->text();
    d.o_client_boot(0,ClientId);//update boot status in bitset table using client id
    QString BootNotification="2,"+ip+",BootNotification,{SerialNumber="+SerialNumber+"|Model="+Model+"|ChargePointSerialNumber="+ChargePointSerialNumber+"|Vendor="+Vendor+"|FirmwareVersion="+FirmwareVersion+"|ICCID="+ICCID+"|IMSI="+IMSI+"|MeterSerialNumber="+MeterSerialNumber+"|MeterType="+MeterType+"}";
    m_webSocket.sendTextMessage(BootNotification);
    qDebug()<<"Message sent :"<<BootNotification;
    SerialNumber="";Model="";ChargePointSerialNumber="";Vendor="";FirmwareVersion="";ICCID="";IMSI="";MeterSerialNumber="";MeterType="",BootNotification="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this,&MainWindow::Received);
}
void MainWindow::DataTransfer()
{
    VendorId=ui->lineEdit_vendorID_Datatransfer_Request->text();
    MessageId=ui->lineEdit_MessageID2_Datatransfer_Request->text();
    Data=ui->lineEdit_Data_Datatransfer_Request->text();
    QString DataTransfer="2,"+ip+",DataTransfer,{VendorId="+VendorId+"|MessageId="+MessageId+"|Data="+Data+"}";
    m_webSocket.sendTextMessage(DataTransfer);
    qDebug()<<"Message sent :"<<DataTransfer;
    VendorId="";MessageId="";Data="";DataTransfer="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this,&MainWindow::Received);
}

void MainWindow::DiagnosticsStatusNotification()
{
    Status=ui->comboBox_Status_diagnosticsnotification_Request->currentText();
    d.o_client_diag(0,ClientId);//update diag status in bitset table using client id
    QString DiagnosticsStatusNotification="2,"+ip+",DiagnosticsStatusNotification,{Status="+Status+"}";
    m_webSocket.sendTextMessage(DiagnosticsStatusNotification);
    qDebug()<<"Message sent :"<<DiagnosticsStatusNotification;
    Status="",DiagnosticsStatusNotification="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this,&MainWindow::Received);
}
void MainWindow::FirmwareStatusNotification()
{
    Status=ui->comboBox_Status_firmwarenotification_Request->currentText();
    d.o_client_firm(0,ClientId);//update firm status in bitset table using client id
    QString FirmwareStatusNotification="2,"+ip+",FirmwareStatusNotification,{Status="+Status+"}";
    m_webSocket.sendTextMessage(FirmwareStatusNotification);
    qDebug()<<"Message sent :"<<FirmwareStatusNotification;
    Status="",FirmwareStatusNotification="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this,&MainWindow::Received);
}
void MainWindow::HeartBeat()
{
    qDebug() <<"HeartBeat loop";
    QString HeartBeat="2,"+ip+",HeartBeat,{RequestHeartBeat}";
    m_webSocket.sendTextMessage(HeartBeat);//update heart status in bitset table using client id
    d.o_client_heartbeat(0,ClientId);
    qDebug() <<"Message sent :"<<HeartBeat;
    HeartBeat="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
}
void MainWindow::MeterValues()
{    
    TimeStamp=CurrentDate;
    serial.start();
    Value=serial.energy;

    if(Value != 0){
        Context=ui->comboBox_context_Metervalue_Request->currentText();
        Format=ui->comboBox_format_Metervalue_Req->currentText();
        Measured=ui->comboBox_measurand_Metervalue_Req->currentText();
        Phase=ui->comboBox_phase_Metervalue_Req->currentText();
        Location=ui->comboBox_location_Metervalue_Req->currentText();
        Unit=ui->comboBox_unit_Metervalue_Req->currentText();
        d.update_client_metervalue(0,ConnectorId);//update meter value status for trigger message from Connectorid details table using connectorid
        QString MeterValues="2,"+ip+",MeterValue,{TimeStamp="+TimeStamp+"|TransactionId="+TransactionId+"|ConnectorId="+ConnectorId+"|Value="+Value+"|Context="+Context+"|Format="+Format+"|Measured="+Measured+"|Phase="+Phase+"|Location="+Location+"|Unit="+Unit+"}";
        m_webSocket.sendTextMessage(MeterValues);
        qDebug()<<"Message sent :"<<MeterValues;
        TimeStamp="";TransactionId="";ConnectorId="";Value="";Context="";Format="";Measured="";Phase="";Location="";Unit="";TransactionId="";MeterValues="";
        connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
    }
    else
    {
//      acc->on_pushButton_power_meter_failure_clicked(CurrentDate,"Power_Meter_Failure","104");
    }

}
void MainWindow::StartTransaction()
{
    if(IdTag!="")
    {
    int DCcharging=0;
//    ConnectorId=ui->lineEdit_ConnectorID_Start_Transaction_Request->text();
    d.get_client_changeavailability(&availability,ConnectorId);// check Availability Status of that connector
    if(availability==1)
    {
        d.get_chargertype_for_starttransaction(ClientId,&ConnectorIdfor7,&ConnectorIdfor8);//get DC connectors
        d.get_client_meter_ONOFF_status(&mON7,ConnectorIdfor7);
        d.get_client_meter_ONOFF_status(&mON8,ConnectorIdfor8);
        qDebug()<<"7 :"<<mON7<<"8 :"<<mON8;
        if((ConnectorIdfor8==ConnectorId)||(ConnectorIdfor7==ConnectorId))
        {
            for(int i=0;i<No_of_Connectors;i++)
            {
                // Get Meterstate from connectorid details table using connectorid
                if(Connectors[i]!=ConnectorId)
                {
                    if(mON[i]==1)
                    {
                        DCcharging=DCcharging+1;
                    }

                }
            }
            if(DCcharging<1)
            {
                //        IdTag=ui->lineEdit_IdTag_Authorize_Request->text();
                        MeterStart="1";
                        d.cp_getReservation(ConnectorId,&IdTag_reserve,&ReservationId);//get resevation id and idtag from reservation details table using connector id
                        d.get_parentidtag_frm_idtag(&ParentIdTag,IdTag);//get parentidtag from user details table using idtag
                        d.get_parentidtag_frm_idtag(&ParentIdTag_reserve,IdTag_reserve);//get parentidtag from user details table using idtag which is reserved
                        d.reserveNow_Status(&reservestatus,ConnectorId);//get reserve status using connector id from connectorid details table
                        if(((reservestatus==1)&&(ParentIdTag==ParentIdTag_reserve))||(reservestatus==0))
                        //if reserved parent idtag must be same thn start charging else if not reserved start charging
                        {
                            TimeStamp=CurrentDate;
                            QString StartTransaction="2,"+ip+",StartTransaction,{ConnectedId="+ConnectorId+"|IdTag="+IdTag+"|MeterStart="+MeterStart+"|ReservationId="+ReservationId+"|TimeStamp="+TimeStamp+"}";
                            m_webSocket.sendTextMessage(StartTransaction);
                            qDebug()<<"Message sent :"<<StartTransaction;
                            IdTag="";MeterStart="";ReservationId="";TimeStamp="";IdTag_reserve="";StartTransaction="";
                            connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
                        }
            }
            else
            {
                QMessageBox::information(this,tr("Sorry!!"),tr("Not Allowed for DC charging"),tr("AC charging in PROGRESS"));
            }
        }
        else
        {
            if((mON7==1)&&(mON8==1))
            {
                QMessageBox::information(this,tr("Sorry!!"),tr("Not Allowed for AC charging"),tr("DC charging in PROGRESS "));
            }
            else
            {

                //        IdTag=ui->lineEdit_IdTag_Authorize_Request->text();
                        MeterStart="1";
                        d.cp_getReservation(ConnectorId,&IdTag_reserve,&ReservationId);//get resevation id and idtag from reservation details table using connector id
                        d.get_parentidtag_frm_idtag(&ParentIdTag,IdTag);//get parentidtag from user details table using idtag
                        d.get_parentidtag_frm_idtag(&ParentIdTag_reserve,IdTag_reserve);//get parentidtag from user details table using idtag which is reserved
                        d.reserveNow_Status(&reservestatus,ConnectorId);//get reserve status using connector id from connectorid details table
                        if(((reservestatus==1)&&(ParentIdTag==ParentIdTag_reserve))||(reservestatus==0))
                        //if reserved parent idtag must be same thn start charging else if not reserved start charging
                        {
                            TimeStamp=CurrentDate;
                            QString StartTransaction="2,"+ip+",StartTransaction,{ConnectedId="+ConnectorId+"|IdTag="+IdTag+"|MeterStart="+MeterStart+"|ReservationId="+ReservationId+"|TimeStamp="+TimeStamp+"}";
                            m_webSocket.sendTextMessage(StartTransaction);
                            qDebug()<<"Message sent :"<<StartTransaction;
                            IdTag="";MeterStart="";ReservationId="";TimeStamp="";IdTag_reserve="";StartTransaction="";
                            connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
                        }
            }
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Unable to proceed Start."),tr("Connector INOPERATIVE"));
    }
    }
}
void MainWindow::RemoteStartTransaction()
{
    if(IdTag!="")
    {
    MeterStart="1";
    d.get_StartTransaction(ConnectorId,&IdTag,&ReservationId);//get resevation id and idtag from transaction table using connector id
    if(ReservationId=='0') // If reservation id is nUll generate transaction id
    {
        d.cp_getReservation(ConnectorId,&IdTag_reserve,&ReservationId);//get reserved idtag from reservation details table using connector id

    }
    d.get_parentidtag_frm_idtag(&ParentIdTag,IdTag);//get parentidtag from user details table using idtag
    d.get_parentidtag_frm_idtag(&ParentIdTag_reserve,IdTag_reserve);//get parentidtag from user details table using idtag which is reserved
    d.reserveNow_Status(&reservestatus,ConnectorId);//get reserve status using connector id from connectorid details table
    if(((reservestatus==1)&&(ParentIdTag==ParentIdTag_reserve))||(reservestatus==0))
    //if reserved parent idtag must be same thn start charging else if not reserved start charging
    {
    TimeStamp=CurrentDate;
    QString StartTransaction="2,"+ip+",StartTransaction,{ConnectedId="+ConnectorId+"|IdTag="+IdTag+"|MeterStart="+MeterStart+"|ReservationId="+ReservationId+"|TimeStamp="+TimeStamp+"}";
    m_webSocket.sendTextMessage(StartTransaction);
    qDebug()<<"Message sent :"<<StartTransaction;
//    d.update_RemoteStart(0,ConnectorId);//update remote start status in connectorid details table using connector id
    ReservationId="";IdTag="";MeterStart="";ReservationId="";TimeStamp="";StartTransaction="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
    }
    }
}
void MainWindow::RemoteStopTransaction()
{
    if(IdTag!="")
    {
    d.get_StartTransaction(ConnectorId,&IdTag,&ReservationId);//get idtag from transaction table using connector id
    d.get_TransactionId(&TransactionId,ConnectorId);//get transaction id from transaction table using connector id
    Reason=ui->comboBox_reason_StopTransaction_Req->currentText();
    MeterStop="1";
    TimeStamp=CurrentDate;
      serial.start();
    Value=serial.energy;
    if(Value!=0)
    {
        Context=ui->comboBox_context_StopTransaction_Req->currentText();
        Format=ui->comboBox_format_StopTransaction_Req->currentText();
        Measured=ui->comboBox_measurand_StopTransaction_Req->currentText();
        Phase=ui->comboBox_phase_StopTransaction_Req->currentText();
        Location=ui->comboBox_location_StopTransaction_Req->currentText();
        Unit=ui->comboBox_unit_StopTransaction_Req->currentText();
        QString StopTransaction="2,"+ip+",StopTransaction,{IdTag="+IdTag+"|Reason="+Reason+"|MeterStop="+MeterStop+"|TimeStamp="+TimeStamp+"|TransactionId="+TransactionId+"|ConnectorId="+ConnectorId+"|Value="+Value+"|Context="+Context+"|Format="+Format+"|Measured="+Measured+"|Phase="+Phase+"|Location="+Location+"|Unit="+Unit+"}";
        m_webSocket.sendTextMessage(StopTransaction);
        qDebug()<<"Message sent :"<<StopTransaction;
        d.update_RemoteStop(0,ConnectorId);//update remote stop status in connectorid details table using connector id
        ReservationId="";IdTag="";Reason="";MeterStop="";TimeStamp="";TransactionId="";ConnectorId="";Context="";Format="";Measured="";Phase="";Location="";Unit="";StopTransaction="";
        connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
    }else
    {
//        acc->on_pushButton_power_meter_failure_clicked(CurrentDate,"Power_Meter_Failure","104");

    }

    }
}

void MainWindow::StatusNotification()
{
    ConnectorId=ui->lineEdit_ConnectorID_StatusNotification_Req->text();
    ErrorCode=ui->comboBox_ErrorCode_StatusNotification_Req->currentText();
    Status=ui->comboBox_Status_StatusNotification_Req->currentText();
    TimeStamp=CurrentDate;
    VendorId=ui->label_vendorID_Datatransfer_Request->text();
    VendorErrorCode=ui->lineEdit_Vendorerrorcode_StatusNotification_Req->text();
    d.o_client_statusnoti(0,ClientId);//update statusnoti status in bitset table using client id
    QString StatusNotification="2,"+ip+",StatusNotification,{ConnectorId="+ConnectorId+"|ErrorCode="+ErrorCode+"|Status="+Status+"|TimeStamp="+TimeStamp+"|VendorId="+VendorId+"|VendorErrorCode="+VendorErrorCode+"}";
    m_webSocket.sendTextMessage(StatusNotification);
    qDebug()<<"Message sent :"<<StatusNotification;
    ConnectorId="";ErrorCode="";Status="";TimeStamp="";VendorId="";VendorErrorCode="";StatusNotification="";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
}
void MainWindow::StopTransaction()
{
    if(IdTag!="")
    {
//    ConnectorId=ui->lineEdit_ConnectorID_StopTransaction_Req->text();
    Reason=ui->comboBox_reason_StopTransaction_Req->currentText();
    MeterStop="1";
    TimeStamp=CurrentDate;
    d.get_TransactionId(&TransactionId,ConnectorId);//get transaction id from transaction table using connector id
    d.get_StartTransaction(ConnectorId,&IdTag_reserve,&ReservationId);//get idtag from transaction table using connector id
    if(IdTag==IdTag_reserve)
    {
        serial.start();

        Value=serial.energy;
        if(Value != 0)
        {
            Context=ui->comboBox_context_StopTransaction_Req->currentText();
            Format=ui->comboBox_format_StopTransaction_Req->currentText();
            Measured=ui->comboBox_measurand_StopTransaction_Req->currentText();
            Phase=ui->comboBox_phase_StopTransaction_Req->currentText();
            Location=ui->comboBox_location_StopTransaction_Req->currentText();
            Unit=ui->comboBox_unit_StopTransaction_Req->currentText();
            QString StopTransaction="2,"+ip+",StopTransaction,{IdTag="+IdTag+"|Reason="+Reason+"|MeterStop="+MeterStop+"|TimeStamp="+TimeStamp+"|TransactionId="+TransactionId+"|ConnectorId="+ConnectorId+"|Value="+Value+"|Context="+Context+"|Format="+Format+"|Measured="+Measured+"|Phase="+Phase+"|Location="+Location+"|Unit="+Unit+"}";
            m_webSocket.sendTextMessage(StopTransaction);
            qDebug()<<"Message sent :"<<StopTransaction;
            ReservationId="";Reason="";MeterStop="";TransactionId="";Context="";Format="";Measured="";Phase="";Location="";Unit="";StopTransaction="";
            connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &MainWindow::Received);
        }
        else
        {
//            acc->on_pushButton_power_meter_failure_clicked(CurrentDate,"Power_Meter_Failure","104");
        }

    }
   }
}







void MainWindow::on_pushButton_Send_Authorization_Req_clicked()
{qDebug()<<"in mainwindow idtag";
    if(networkresult==true)
    {   int ans;
        auth->check_list(IdTag,&ans);
        if(ans==1)
        {
//            ***IdTag=ui->lineEdit_IdTag_Authorize_Request->text();
            auth->on_pushButton_clicked();
            auth->get_list(IdTag,&ExpiryDate);
            QDateTime current=QDateTime::fromString(CurrentDate,"yyyy-MM-dd HH:mm:ss");
            QDateTime expiry=QDateTime::fromString(ExpiryDate,"yyyy-MM-dd HH:mm:ss");
            int currentsecs=current.toMSecsSinceEpoch();
            int expirysecs=expiry.toMSecsSinceEpoch();
            qDebug()<<ExpiryDate;
            qDebug()<<CurrentDate;
            qDebug()<<expiry;
            qDebug()<<current;
            if(currentsecs>expirysecs)
            {
                Authorize();
            }
            else
            {
                Authorizevalue=1;
            }
        }
        else if(ans==0)
        {
            Authorize();
        }
    }
    else
    {
//        ***IdTag=ui->lineEdit_IdTag_Authorize_Request->text();
        auth->on_pushButton_clicked();
        auth->get_list(IdTag,&ExpiryDate);
        QDateTime current=QDateTime::fromString(CurrentDate,"yyyy-MM-dd HH:mm:ss");
        QDateTime expiry=QDateTime::fromString(ExpiryDate,"yyyy-MM-dd HH:mm:ss");
        int currentsecs=current.toMSecsSinceEpoch();
        int expirysecs=expiry.toMSecsSinceEpoch();
        qDebug()<<ExpiryDate;
        qDebug()<<CurrentDate;
        qDebug()<<expiry;
        qDebug()<<current;
        if(currentsecs<expirysecs)
        {
            //disable start stop transactions
        }
    }
}
void MainWindow::on_pushButton_send_bootNotification_req_clicked()
{
    if(networkresult==true)
    {
    BootNotification();
    }
}
void MainWindow::on_pushButton_Send_Data_transfer_req_clicked()
{
    if(networkresult==true)
    {
    DataTransfer();
    }
}
void MainWindow::on_pushButton_Send_DiagnosticsStatusNotification_Req_clicked()
{
    if(networkresult==true)
    {
    DiagnosticsStatusNotification();
    }
}
void MainWindow::on_pushButton_frimware_notification_Req_clicked()
{
    if(networkresult==true)
    {
    FirmwareStatusNotification();
    }
}
void MainWindow::on_pushButton_Send_HeartBeat_Req_clicked()
{
    if(networkresult==true)
    {
    HeartBeat();
    }
}
void MainWindow::on_pushButton_Send_Meter_Values_Req_clicked()
{
    if(networkresult==true)
    {
    MeterValues();
    }
    else
    {
        TimeStamp=CurrentDate;
        Value=ui->lineEdit_value_Metervalue_Request->text();
        Context=ui->comboBox_context_Metervalue_Request->currentText();
        Format=ui->comboBox_format_Metervalue_Req->currentText();
        Measured=ui->comboBox_measurand_Metervalue_Req->currentText();
        Phase=ui->comboBox_phase_Metervalue_Req->currentText();
        Location=ui->comboBox_location_Metervalue_Req->currentText();
        Unit=ui->comboBox_unit_Metervalue_Req->currentText();
        t->on_pushButton_clicked();
        t->update_list_metervalues(TimeStamp,Value,Context,Format,Measured,Phase,Location,Unit,TransactionId);
        TimeStamp="";TransactionId="";ConnectorId="";Value="";Context="";Format="";Measured="";Phase="";Location="";Unit="";TransactionId="";
    }
}
void MainWindow::on_pushButton_Send_Start_Transaction_Request_clicked()
{
    if(networkresult==true)
    {
    StartTransaction();
    }
    else
    {
        ConnectorId=ui->lineEdit_ConnectorID_Start_Transaction_Request->text();
        MeterStart="1";
        TimeStamp=CurrentDate;
        quint16 Transaction= QRandomGenerator::global()->generate();
        TransactionId = QString::number(Transaction);
        t->show();
        t->on_pushButton_clicked();
        t->update_list_start(TransactionId,ConnectorId,IdTag,MeterStart,TimeStamp);
        on_pushButton_Send_Meter_Values_Req_clicked();
        IdTag="";MeterStart="";TimeStamp="";
    }
}
void MainWindow::on_pushButton_Status_Notification_Request_clicked()
{
    if(networkresult==true)
    {
    StatusNotification();
    }
}
void MainWindow::on_pushButton_Send_Stop_Transaction_Req_clicked()
{
    if(networkresult==true)
    {
    StopTransaction();
    }
    else
    {
        ConnectorId=ui->lineEdit_ConnectorID_StopTransaction_Req->text();
        Reason=ui->comboBox_reason_StopTransaction_Req->currentText();
        MeterStop="1";
        TimeStamp=CurrentDate;
        t->get_list(ConnectorId,&TransactionId);
        Value=ui->lineEdit_value_StopTransaction_Req->text();
        Context=ui->comboBox_context_StopTransaction_Req->currentText();
        Format=ui->comboBox_format_StopTransaction_Req->currentText();
        Measured=ui->comboBox_measurand_StopTransaction_Req->currentText();
        Phase=ui->comboBox_phase_StopTransaction_Req->currentText();
        Location=ui->comboBox_location_StopTransaction_Req->currentText();
        Unit=ui->comboBox_unit_StopTransaction_Req->currentText();
        t->on_pushButton_clicked();
        t->update_list_stop(TransactionId,ConnectorId,IdTag,TimeStamp,MeterStop,Reason,Value,Context,Format,Measured,Phase,Location,Unit);
        IdTag="";Reason="";MeterStop="";TimeStamp="";TransactionId="";ConnectorId="";Value="";Context="";Format="";Measured="";Phase="";Location="";Unit="";
     }
}
void MainWindow::on_pushButton_Authorize_clicked()
{
    ui->widget_authorize->show();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();

}

void MainWindow::on_pushButton_Bootnotification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->show();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Datatransfer_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->show();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Diagnostics_Status_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->show();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Firmware_Status_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->show();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Heartbeat_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->show();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Metervalues_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->show();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_StartTransaction_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->show();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Status_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->show();
    ui->widget_StopTransaction->hide();
}

void MainWindow::on_pushButton_Stop_Notification_clicked()
{
    ui->widget_authorize->hide();
    ui->widget_bootnotification->hide();
    ui->widget_DataTransfer->hide();
    ui->widget_DiagnosticsStatusNotification->hide();
    ui->widget_FirmwareStatusNotification->hide();
    ui->widget_HeartBeat->hide();
    ui->widget_MeterValues->hide();
    ui->widget_Start_Transaction->hide();
    ui->widget_StatusNotification->hide();
    ui->widget_StopTransaction->show();
}

void MainWindow::on_start_con_1_clicked()
{
    ConnectorId=Connectors[0];
    StartTransaction();
}

void MainWindow::on_start_con_2_clicked()
{
    ConnectorId=Connectors[1];
    StartTransaction();
}

void MainWindow::on_start_con_3_clicked()
{
    ConnectorId=Connectors[2];
    StartTransaction();
}
void MainWindow::on_start_con_4_clicked()
{
    ConnectorId=Connectors[3];
    StartTransaction();
}

void MainWindow::on_start_con_5_clicked()
{
    ConnectorId=Connectors[4];
    StartTransaction();
}

void MainWindow::on_start_con_6_clicked()
{
    ConnectorId=Connectors[5];
    StartTransaction();
}
void MainWindow::on_start_con_7_clicked()
{
    ConnectorId=Connectors[6];
    StartTransaction();
}

void MainWindow::on_start_con_8_clicked()
{
    ConnectorId=Connectors[7];
    StartTransaction();
}


void MainWindow::on_stop_con_1_clicked()
{
    ConnectorId=Connectors[0];
    StopTransaction();
}

void MainWindow::on_stop_con_2_clicked()
{
    ConnectorId=Connectors[1];
    StopTransaction();
}

void MainWindow::on_stop_con_3_clicked()
{
    ConnectorId=Connectors[2];
    StopTransaction();
}
void MainWindow::on_stop_con_4_clicked()
{
    ConnectorId=Connectors[3];
    StopTransaction();
}

void MainWindow::on_stop_con_5_clicked()
{
    ConnectorId=Connectors[4];
    StopTransaction();
}

void MainWindow::on_stop_con_6_clicked()
{
    ConnectorId=Connectors[5];
    StopTransaction();
}void MainWindow::on_stop_con_7_clicked()
{
    ConnectorId=Connectors[6];
    StopTransaction();
}

void MainWindow::on_stop_con_8_clicked()
{
    ConnectorId=Connectors[7];
    StopTransaction();
}
