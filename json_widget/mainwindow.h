#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "database.h"
#include <QMainWindow>
#include <QtWebSockets/QWebSocket>
#include <QWebSocketServer>
#include <QMessageBox>
#include <QtNetwork>
#include "filereadwrite.h"
#include "authorizeoffline.h"
#include "transactionoffline.h"
#include "serialmodbus.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    FileReadWrite *r=new FileReadWrite();
    AuthorizeOffline *auth=new AuthorizeOffline();
    TransactionOffline *t=new TransactionOffline();
    serialmodbus serial;

    database d;

    bool networkresult;
    bool networkflag = true;
    int No_of_Connectors,Authorizevalue=0;
    QString Connectors[10];

    QString CurrentDate;
    QWebSocketServer *m_pWebSocketServer;
    QString message,ClientId,ip,gmt;
    QString msg_Id,uniq_Id,Action,Payload,result,PayloadHead;
    QString ExpiryDate,Status,IdTag,Interval,SerialNumber,number,ConnectorIdfor7,ConnectorIdfor8,
    Model,ChargePointSerialNumber,Vendor,FirmwareVersion,ICCID,IMSI,MeterSerialNumber,MeterType,CurrentTime,TimeStamp,
    TransactionId,MeterValue,ConnectorId,Value,Context,Format,Measured,Phase,Location,Unit,MeterStart,
    ReservationId,Reason,MeterStop,ParentIdTag,Transaction,VendorId,MessageId,Data,VendorErrorCode,ErrorCode,IdTag_reserve,ParentIdTag_reserve;
    int boot,diag,firm,heart,statusnoti,availability,reservestatus,client_id;
    int start[10],avail[10],stop[10],res[10],mON[10],mtrval[10],mON7,mON8,interval;
    QTimer *timer,*timer2;

    void Sub3();

    void Received(QString message);
    void Authorize();
    void BootNotification();
    void DataTransfer();
    void DiagnosticsStatusNotification();
    void FirmwareStatusNotification();
    void HeartBeat();
    void MeterValues();
    void StartTransaction();
    void StatusNotification();
    void StopTransaction();


    void RemoteStartTransaction();
    void RemoteStopTransaction();

public slots:
    void func();

    void heartbeatloop();

    void closed();

    void on_pushButton_Authorize_clicked();

    void on_pushButton_Bootnotification_clicked();

    void on_pushButton_Datatransfer_clicked();

    void on_pushButton_Diagnostics_Status_Notification_clicked();

    void on_pushButton_Firmware_Status_Notification_clicked();

    void on_pushButton_Heartbeat_clicked();

    void on_pushButton_Metervalues_clicked();

    void on_pushButton_StartTransaction_clicked();

    void on_pushButton_Status_Notification_clicked();

    void on_pushButton_Stop_Notification_clicked();

    void on_pushButton_StartChargePoint_clicked();

    void on_pushButton_StopChargePoint_clicked();

    void on_pushButton_Send_Authorization_Req_clicked();

    void on_pushButton_send_bootNotification_req_clicked();

    void on_pushButton_Send_Data_transfer_req_clicked();

    void on_pushButton_Send_DiagnosticsStatusNotification_Req_clicked();

    void on_pushButton_frimware_notification_Req_clicked();

    void on_pushButton_Send_HeartBeat_Req_clicked();

    void on_pushButton_Send_Meter_Values_Req_clicked();

    void on_pushButton_Send_Start_Transaction_Request_clicked();

    void on_pushButton_Status_Notification_Request_clicked();

    void on_pushButton_Send_Stop_Transaction_Req_clicked();


public slots:
    void on_start_con_1_clicked();

    void on_start_con_2_clicked();

    void on_start_con_3_clicked();

    void on_start_con_4_clicked();

    void on_start_con_5_clicked();

    void on_start_con_6_clicked();

    void on_start_con_7_clicked();

    void on_start_con_8_clicked();

    void on_stop_con_1_clicked();

    void on_stop_con_2_clicked();

    void on_stop_con_3_clicked();

    void on_stop_con_4_clicked();

    void on_stop_con_5_clicked();

    void on_stop_con_6_clicked();

    void on_stop_con_7_clicked();

    void on_stop_con_8_clicked();

public:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
    QList<QWebSocket *> m_clients;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
