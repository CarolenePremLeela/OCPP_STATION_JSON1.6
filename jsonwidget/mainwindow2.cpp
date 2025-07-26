#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),QWebSocketServer::NonSecureMode, this)),ui(new Ui::MainWindow2)
{
    ui->setupUi(this);
    on_pushButton_StartChargePoint_clicked();
}

MainWindow2::~MainWindow2()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
    delete ui;
}
void MainWindow2::on_pushButton_StartChargePoint_clicked()
{
    ui->pushButton_StartChargePoint->setDisabled(1);
    ui->pushButton_StopChargePoint->setEnabled(1);
    QString portstr=ui->lineEdit_port->text();//fetching Port
    int port=portstr.toInt();
    foreach(const QHostAddress &address,QNetworkInterface::allAddresses())
    if(address.protocol()==QAbstractSocket::IPv4Protocol && address!=QHostAddress(QHostAddress::LocalHost))
    {
//        host=address.toString();
        host="49.206.200.225";

        ui->lineEdit_server_ip->setText(host);
    }
        if (m_pWebSocketServer->listen( QHostAddress(host), port))
        {
            qDebug() << "Echoserver listening on port" << port;
            connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this, &MainWindow2::onNewConnection);//if New Socket Connected, go new connection
        }
}

void MainWindow2::on_pushButton_StopChargePoint_clicked()
{
    ui->pushButton_StopChargePoint->setDisabled(1);
    ui->pushButton_StartChargePoint->setEnabled(1);
    m_pWebSocketServer->close();// Close Server
}
void MainWindow2::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();// Connection to websocket with server
    QString url= pSocket->resourceName();// Get the Url connected
    QStringList lines = url.split('/');
    QString ipvector=lines.at(2);
    cp_name=lines.at(4);// Central System Server Name from Url
    lines = ipvector.split(':');
    QString ip=lines.at(0);// Ip from Url
    QString port=lines.at(1);// Port From Url
    qDebug()<<cp_name <<"is connected.";
    connect(pSocket, &QWebSocket::textMessageReceived, this, &MainWindow2::processTextMessage);// If text Message received from socket, go to process text message
    connect(pSocket, &QWebSocket::disconnected, this, &MainWindow2::socketDisconnected);// If disconnected, go to socket disconnected
}

void MainWindow2::processTextMessage(QString message)
{
    if(message!=NULL)
    {
      QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
      qDebug() << "Message received:" << message;
      QStringList lines = message.split(',');
      msg_Id =lines.at(0);
      uniq_Id=lines.at(1);
      PayloadHead =lines.at(2);
      Payload=lines.at(3);
      if(msg_Id=="2")
      {
          Sub2();
          pClient->sendTextMessage(result);// Reply to the Socket
          result="";
      }
}
}
void MainWindow2::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << cp_name <<"is Disconnected.";
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
void MainWindow2::Sub2()//If Message Id = 2;
{
    if(PayloadHead=="CancelReservation")
    {
            QStringList lines = Payload.split('{');
            QString y=lines.at(1);
            lines = y.split('}');
             y=lines.at(0);
            lines = y.split('=');
            QString x1=lines.at(0);
            QString x2=lines.at(1);
            if(x1=="ReservationId")
            {
                ReservationId.append(x2);
                d.update_reservestatus(0,ReservationId);//update reserve status from connectorid details table using reservation id
                d.dropReservationId(ReservationId);//delete row with reservation id in reservation details table
                CancelReservation();
                ReservationId="";
            }
        }
    if(PayloadHead=="ChangeAvailability")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = b2.split('}');
        b2=lines.at(0);
        if(a1=="ConnectorId")
        {
            ConnectorId.append(a2);
            Type.append(b2);
            d.get_client_changeavailability(&chngavail,ConnectorId);// Get change availability status from Connector id details table using ConnectorId
            if((Type=="INOPERATIVE")&&(chngavail==1))//if type=inoperative and availability status=operative update
            {
                d.update_client_changeavailability(0,ConnectorId);//update availability status using connectorid
                Status="Accepted";
            }
            if((Type=="OPERATIVE")&&(chngavail==0))//if type=operative and availability status=inoperative update
            {
                d.update_client_changeavailability(1,ConnectorId);//update availability status using connectorid
                Status="Accepted";
            }
            ChangeAvailability();
            d.update_client_statusnoti(1,ConnectorId);
            ConnectorId="";Type="";
        }
    }
    if(PayloadHead=="ChangeConfiguration")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = b2.split('}');
        b2=lines.at(0);
        if(a1=="Key")
        {
            Key.append(a2);
            NewValue.append(b2);
            w->r->on_pushButton_clicked();//load all datas in txt file into table
            w->r->change_key(Key,NewValue);//replace the new value in the key index and store in same txt file
            ChangeConfiguration();
            Key="";NewValue="";
        }
    }
    if(PayloadHead=="ClearCache")
    {
        QStringList lines = Payload.split('}');
        QString u=lines.at(0);
        lines=u.split('{');
        u=lines.at(1);
        if(u=="RequestClearCache")
        {//clear all objects in all class files
//                QCache<int,MainWindow> cache;
//                cache.clear();
                QCache<int,MainWindow2> cache2;
                cache2.clear();
                QCache<int,FileReadWrite> cache3;
                cache3.clear();
            ClearCache();
        }
    }
    /*if(PayloadHead=="ClearChargingProfile")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = c2.split('}');
        c2=lines.at(0);
        if(a1=="ID")
        {
            IdTag.append(a2);
            ConnectorId.append(b2);
            PurposeType.append(c2);
            ClearChargingProfile();
            IdTag="";ConnectorId="";PurposeType="";
        }
    }*/
    if(PayloadHead=="DataTransfer")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = c2.split('}');
        c2=lines.at(0);
        if(a1=="VendorId")
        {
            VendorId.append(a2);
            MessageId.append(b2);
            Data.append(c2);
            DataTransfer();
            VendorId="";MessageId="";Data="";

        }
    }
    /*if(PayloadHead=="GetCompositeSchedule")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = c2.split('}');
        c2=lines.at(0);
        if(a1=="ConnectorId")
        {
            ConnectorId.append(a2);
            Duration.append(b2);
            ChargingUnitType.append(c2);
            GetCompositeSchedule();
            ChargingUnitType="";ConnectorId="";Duration="";
        }
    }*/
    if(PayloadHead=="GetConfiguration")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
         y=lines.at(0);
        lines = y.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        if(x1=="Key")
        {
            Key.append(x2);
            w->r->on_pushButton_clicked();//load all datas in txt file into table
            w->r->get_no_of_connectors(Key,&Value);
            GetConfiguration();
        }
    }
    if(PayloadHead=="GetDiagnostics")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        QString e=lines.at(3);
        QString f=lines.at(4);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = e.split('=');
        QString e2=lines.at(1);
        lines = f.split('=');
        QString f2=lines.at(1);
        lines = f2.split('}');
        f2=lines.at(0);
        if(a1=="Location")
        {
            Location.append(a2);
            Retries.append(b2);
            RetryInterval.append(c2);
            StartTime.append(e2);
            StopTime.append(f2);
            QStringList FILEName=Location.split('/');
            int count = Location.count('/');
            FILENAME=FILEName.at(count);
            tcps->start(uniq_Id,8888,"C:/Users/firstcall/Desktop/receivedfile/"+FILENAME);
            GetDiagnostics(FILENAME);
            tcps->~clientsocket();
            Location="";Retries="";RetryInterval="";StartTime="";StopTime="";FILENAME="";
        }
    }
    if(PayloadHead=="GetLocalListVersion")
    {
       QStringList lines = Payload.split('}');
                QString u=lines.at(0);
                lines=u.split('{');
                u=lines.at(1);
                if(u=="RequestLocalListVersion")
                {
                    GetLocalListVersion();
                }
    }
    if(PayloadHead=="RemoteStartTransaction")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        QString e=lines.at(3);
        QString f=lines.at(4);
        QString g=lines.at(5);
        QString h=lines.at(6);
        QString i=lines.at(7);
        QString j=lines.at(8);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = e.split('=');
        QString e2=lines.at(1);
        lines = f.split('=');
        QString f2=lines.at(1);
        lines = g.split('=');
        QString g2=lines.at(1);
        lines = h.split('=');
        QString h2=lines.at(1);
        lines = i.split('=');
        QString i2=lines.at(1);
        lines = j.split('=');
        QString j2=lines.at(1);
        lines = j2.split('}');
        j2=lines.at(0);
        if(a1=="ConnectorId")
        {
            ConnectorId.append(a2);
            IdTag.append(b2);
            TransactionId.append(c2);
            StackLevel.append(e2);
            UnitType.append(f2);
            MinChargingRate.append(g2);
            StartPeriod.append(h2);
            Limit.append(i2);
            NumberPhases.append(j2);
            d.Insert_RemoteStartTransaction(ConnectorId,IdTag,TransactionId);//insert into transaction table
            RemoteStartTransaction();
            d.update_RemoteStart(1,ConnectorId);//update remote start status in connectorid details table using connector id
            ConnectorId="";IdTag="";TransactionId="";StackLevel="";UnitType="";MinChargingRate="";StartPeriod="";Limit="";NumberPhases="";
        }
    }
    if(PayloadHead=="RemoteStopTransaction")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
         y=lines.at(0);
        lines = y.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        if(x1=="TransactionId")
        {
            TransactionId.append(x2);
            RemoteStopTransaction();
            d.get_conid_frm_tranid(&ConnectorId,TransactionId);//get connectorid using transaction id from transaction table
            d.update_RemoteStop(1,ConnectorId);//update remote stop status in connectorid details table using connector id
            TransactionId="";ConnectorId="";
        }
    }
    if(PayloadHead=="ReserveNow")
    {
      QStringList lines = Payload.split('{');
                   QString y=lines.at(1);
                   lines = y.split('|');
                   QString a=lines.at(0);
                   QString b=lines.at(1);
                   QString c=lines.at(2);
                   QString e=lines.at(3);
                   QString f=lines.at(4);
                   QString g=lines.at(5);
                   QString h=lines.at(6);
                   lines = a.split('=');
                   QString a1=lines.at(0);
                   QString a2=lines.at(1);
                   lines = b.split('=');
                   QString b2=lines.at(1);
                   lines = c.split('=');
                   QString c2=lines.at(1);
                   lines = e.split('=');
                   QString e2=lines.at(1);
                   lines = f.split('=');
                   QString f2=lines.at(1);
                   lines = g.split('=');
                   QString g2=lines.at(1);
                   lines = h.split('=');
                   QString h2=lines.at(1);
                   lines = h2.split('}');
                   h2=lines.at(0);
                   if(a1=="ConnectorId")
                   {
                       ConnectorId.append(a2);
                       IdTag.append(b2);
                       ParentIdTag.append(c2);
                       ReservedTime.append(e2);
                       ExpiryTime.append(f2);
                       Duration.append(g2);
                       ReservationId.append(h2);
                       d.get_client_changeavailability(&availability,ConnectorId);// Get change availability status from Connector id details table using ConnectorId
                       if(availability==1)
                       {d.reserveNow_Status(&res,ConnectorId);// Get reserve status from Connector id details table using ConnectorId
                             if(res==0)
                             {d.get_client_meter_ONOFF_status(&mON,ConnectorId);// Get meter on-off status from Connector id details table using ConnectorId
                                 if(mON==0)
                                 {
                                     QDateTime issueDate = QDateTime::currentDateTime();
                                     CurrentTime=issueDate.toString("yyyy-MM-dd HH:mm:ss");
                                    d.reservebackup(ReservationId,IdTag,ConnectorId,CurrentTime,ReservedTime,ExpiryDate,Duration);//insert into reservatio backup table
                                    d.update_reservestatus(1,ReservationId);//update reserve status in connectorid details table using reservation id
                                    ReservationId="";IdTag="";ParentIdTag="";ExpiryDate="";ReservedTime="";ExpiryTime="";Duration="";CurrentTime="";
                                    Status="Accepted";
                                 }
                                 else{Status="Occupied";}
                             }
                             else{Status="Rejected";}
                      }
                      else{Status="Unavailable";}
                   ReserveNow();
                   }
            }
    if(PayloadHead=="Reset")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
         y=lines.at(0);
        lines = y.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        if(x1=="Type")
        {
            Type.append(x2);
            if((Type=="Hard")||(Type=="Soft"))//if type hard or soft comes accepted
            {
                Status="Accepted";
            }
            else
            {
                Status="Rejected";
            }
            Reset();
            if(Type=="Hard")//if hard restart system
            {
                QProcess::startDetached("shutdown /r");
            }
            else if(Type=="Soft")//if soft restart restart execution file
            {
                connect(&soft_reset_timer,SIGNAL(timeout()),this,SLOT(softreset()));
                soft_reset_timer.start(100);//restart exe after 100 secs delay
            }
            Type="";
        }
    }
    if(PayloadHead=="SendLocalList")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = c2.split('}');
        c2=lines.at(0);
        if(a1=="ListVersion")
        {
            LocalListVersion.append(a2);
            UpdateType.append(b2);
            QString Stream;
            Stream.append(c2);
            if(UpdateType=="Full")
            {
             w->auth->Write_empty();
             w->auth->insert_list("IdTag","ExpiryDate","ParentIdTag");
                QStringList rows=Stream.split('\n');
                int numRows = rows.count();
                int numColumns = rows.first().count('\t')+1;
                for (int i = 0; i < numRows; ++i)
                {
                    QStringList columns = rows[i].split('\t');
                    for (int j = 0; j < numColumns; ++j)
                    {
                        columns[j];
                    }
                    qDebug()<<columns[0]<<columns[1]<<columns[2];
                    w->auth->insert_list(columns[0],columns[1],columns[2]);
                }
                w->auth->WriteData();
                Status="Accepted";
            }
            if(UpdateType=="Differential")
            {
                QStringList rows=Stream.split('\n');
                int numRows = rows.count();
                int numColumns = rows.first().count('\t')+1;
                for (int i = 0; i < numRows; ++i)
                {
                    QStringList columns = rows[i].split('\t');
                    for (int j = 0; j < numColumns; ++j)
                    {
                        columns[j];
                    }
                    qDebug()<<columns[0]<<columns[1]<<columns[2];
                    w->auth->on_pushButton_clicked();
                    w->auth->update_list(columns[0],columns[1],columns[2]);
                }
                Status="Accepted";
            }
            SendLocalList();
            UpdateType="";LocalListVersion="";Stream="";
        }
    }
    /*if(PayloadHead=="SetChargingProfile")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        QString c=lines.at(2);
        QString e=lines.at(3);
        QString f=lines.at(4);
        QString g=lines.at(5);
        QString h=lines.at(6);
        QString i=lines.at(7);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = c.split('=');
        QString c2=lines.at(1);
        lines = e.split('=');
        QString e2=lines.at(1);
        lines = f.split('=');
        QString f2=lines.at(1);
        lines = g.split('=');
        QString g2=lines.at(1);
        lines = h.split('=');
        QString h2=lines.at(1);
        lines = i.split('=');
        QString i2=lines.at(1);
        lines = i2.split('}');
        i2=lines.at(0);
        if(a1=="ConnectorId")
        {
            ConnectorId.append(a2);
            IdTag.append(b2);
            StackLevel.append(c2);
            UnitType.append(e2);
            MinChargingRate.append(f2);
            StartPeriod.append(g2);
            Limit.append(h2);
            NumberPhases.append(i2);
            SetChargingProfile();
            ConnectorId="";IdTag="";StackLevel="";UnitType="";MinChargingRate="";StartPeriod="";Limit="";NumberPhases="";
        }
    }*/
    if(PayloadHead=="TriggerMessage")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('|');
        QString a=lines.at(0);
        QString b=lines.at(1);
        lines = a.split('=');
        QString a1=lines.at(0);
        QString a2=lines.at(1);
        lines = b.split('=');
        QString b2=lines.at(1);
        lines = b2.split('}');
        b2=lines.at(0);
        if(a1=="RequestMessage")
        {
            RequestMessage.append(a2);
            ConnectorId.append(b2);
            TriggerMessage();
            if(RequestMessage=="BootNotification")
            {
                 d.update_client_boot(1,ConnectorId);//update boot in bitset table using connector id
            }
            if(RequestMessage=="DiagnosticsStatusNotification")
            {
                 d.update_client_diag(1,ConnectorId);//update diag in bitset table using connector id
            }
            if(RequestMessage=="FirmwareStatusNotification")
            {
                 d.update_client_firm(1,ConnectorId);//update firm in bitset table using connector id
            }
            if(RequestMessage=="MeterValues")
            {
                d.update_client_metervalue(1,ConnectorId);//update meter value status from Connectorid details table using connectorid
            }
            if(RequestMessage=="HeartBeat")
            {
                d.update_client_heartbeat(1,ConnectorId);//update heart in bitset table using connector id
            }
            if(RequestMessage=="StatusNotification")
            {
                d.update_client_statusnoti(1,ConnectorId);//update statusnoti in bitset table using connector id
            }
            RequestMessage="";ConnectorId="";
        }
    }
    if(PayloadHead=="UnlockConnector")
    {
        QStringList lines = Payload.split('{');
        QString y=lines.at(1);
        lines = y.split('}');
         y=lines.at(0);
        lines = y.split('=');
        QString x1=lines.at(0);
        QString x2=lines.at(1);
        if(x1=="ConnectorId")
        {
            ConnectorId.append(x2);
            d.get_client_meter_ONOFF_status(&availability,ConnectorId);
            if(availability==1)
            {
                d.update_RemoteStop(1,ConnectorId);//update remote stop status in connectorid details table using connector id
                Status="Accepted";
            }
            UnlockConnector();
            ConnectorId="";
        }
    }
    if(PayloadHead=="UpdateFirmware")
    {
        QStringList lines = Payload.split('{');
                     QString y=lines.at(1);
                     lines = y.split('|');
                     QString a=lines.at(0);
                     QString b=lines.at(1);
                     QString c=lines.at(2);
                     QString e=lines.at(3);
                     lines = a.split('=');
                     QString a1=lines.at(0);
                     QString a2=lines.at(1);
                     lines = b.split('=');
                     QString b2=lines.at(1);
                     lines = c.split('=');
                     QString c2=lines.at(1);
                     lines = e.split('=');
                     QString e2=lines.at(1);
                     lines = e2.split('}');
                     e2=lines.at(0);
                     if(a1=="Location")
                     {
                         Location.append(a2);
                         Retries.append(b2);
                         RetrieveDate.append(c2);
                         RetryInterval.append(e2);
                             QProcess *process = new QProcess(this);
                             process->execute(Location);
                         UpdateFirmware();
                         Location="";Retries="";RetryInterval="";RetrieveDate="";
                     }
    }
}
void MainWindow2::softreset()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
void MainWindow2::CancelReservation()
{
    Status=ui->comboBox_Status_CancelReservation_Response->currentText();
    result="3,"+uniq_Id+",CancelReservation,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::ChangeAvailability()
{
    if(Status=="Accepted")
    {
        Status="Accepted";
    }else
    {
        Status="Rejected";
    }
    result="3,"+uniq_Id+",ChangeAvailability,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::ChangeConfiguration()
{
    Status=ui->comboBox_Status_Change_Configuration_Response->currentText();
    result="3,"+uniq_Id+",ChangeConfiguration,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::ClearCache()
{
    Status=ui->comboBox_Status_Clear_Cache_Response->currentText();
    result="3,"+uniq_Id+",ClearCache,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
/*void MainWindow2::ClearChargingProfile()
{
    Status=ui->comboBox_Status_Clear_Changing_Profile_Response->currentText();
    result="3,"+uniq_Id+",ClearChargingProfile,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}*/
void MainWindow2::DataTransfer()
{
}
/*void MainWindow2::GetCompositeSchedule()
{
}*/
void MainWindow2::GetConfiguration()
{
    result="3,"+uniq_Id+",GetConfiguration,{Key="+Key+"|Value="+Value+"}";
    qDebug()<<"Message sent :"<<result;
    Key="";Value="";
}
void MainWindow2::GetDiagnostics(QString FILENAME)
{
    result="3,"+uniq_Id+",GetDiagnostics,{FileName="+FILENAME+"}";
    qDebug()<<"Message sent :"<<result;
    FILENAME="";
}
void MainWindow2::GetLocalListVersion()
{
//    auth->on_pushButton_clicked();
//    LocalListVersion=auth->getlocalistversion;
    result="3,"+uniq_Id+",GetLocalListVersion,{LocalListVersion="+LocalListVersion+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::RemoteStartTransaction()
{
    Status=ui->comboBox_Status_Remote_Start_Transaction_Response->currentText();
    result="3,"+uniq_Id+",RemoteStartTransaction,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::RemoteStopTransaction()
{
    Status=ui->comboBox_Status_Remote_Stop_Transaction_Response->currentText();
    result="3,"+uniq_Id+",RemoteStopTransaction,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::ReserveNow()
{
    result="3,"+uniq_Id+",ReserveNow,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::Reset()
{
    if(Status=="Accepted")
    {
        Status="Accepted";
    }else
    {
        Status="Rejected";
    }
    result="3,"+uniq_Id+",Reset,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::SendLocalList()
{
    if(Status=="Accepted")
    {
        Status="Accepted";
    }else
    {
        Status="Rejected";
    }
    result="3,"+uniq_Id+",SendLocalList,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
/*void MainWindow2::SetChargingProfile()
{
    Status=ui->comboBox_Status_Set_Charging_Profile_Response->currentText();
    result="3,"+uniq_Id+",SetChargingProfile,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}*/
void MainWindow2::TriggerMessage()
{
    Status=ui->comboBox_Status_TriggerMessage_Response->currentText();
    result="3,"+uniq_Id+",TriggerMessage,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::UnlockConnector()
{
    if(Status=="Accepted")
    {
        Status="Accepted";
    }else
    {
        Status="Rejected";
    }
    result="3,"+uniq_Id+",UnlockConnector,{Status="+Status+"}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}
void MainWindow2::UpdateFirmware()
{
    result="3,"+uniq_Id+",UpdateFirmware,{Acknowledgementfromserver}";
    qDebug()<<"Message sent :"<<result;
    Status="";
}








void MainWindow2::on_pushButton_Cancel_Reservation_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow2::on_pushButton_Change_Availability_clicked()
{
 ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow2::on_pushButton_Change_Configuration_clicked()
{
 ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow2::on_pushButton_Clear_Cache_clicked()
{
   ui->stackedWidget->setCurrentIndex(3);
}

/*void MainWindow2::on_pushButton_Clear_Charging_Profile_clicked()
{
   ui->stackedWidget->setCurrentIndex(4);
}*/

void MainWindow2::on_pushButton_DataTransfer_clicked()
{
  ui->stackedWidget->setCurrentIndex(5);
}

/*void MainWindow2::on_pushButton_Get_Composite_Schedule_clicked()
{
   ui->stackedWidget->setCurrentIndex(6);
}*/

void MainWindow2::on_pushButton_Get_Configuration_clicked()
{
  ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow2::on_pushButton_Get_Diagnostics_clicked()
{
 ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow2::on_pushButton_Get_Local_List_Version_clicked()
{
  ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow2::on_pushButton_Remote_Start_Transaction_clicked()
{
   ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow2::on_pushButton_Remote_Stop_Transaction_clicked()
{
   ui->stackedWidget->setCurrentIndex(11);
}

void MainWindow2::on_pushButton_Reserve_Now_clicked()
{
  ui->stackedWidget->setCurrentIndex(12);
}

void MainWindow2::on_pushButton_Reset_clicked()
{
  ui->stackedWidget->setCurrentIndex(13);
}

void MainWindow2::on_pushButton_Send_Local_List_clicked()
{
  ui->stackedWidget->setCurrentIndex(14);
}

/*void MainWindow2::on_pushButton_Set_Charging_Profile_clicked()
{
   ui->stackedWidget->setCurrentIndex(15);
}*/

void MainWindow2::on_pushButton_Trigger_Message_clicked()
{
 ui->stackedWidget->setCurrentIndex(16);
}

void MainWindow2::on_pushButton_Unlock_Connector_clicked()
{
  ui->stackedWidget->setCurrentIndex(17);
}

void MainWindow2::on_pushButton_Update_Firmware_clicked()
{
   ui->stackedWidget->setCurrentIndex(18);
}

