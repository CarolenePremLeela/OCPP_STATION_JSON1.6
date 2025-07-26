#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H
#include <QtWebSockets/QWebSocket>
#include <QWebSocketServer>
#include <QMainWindow>
#include "database.h"
#include <QNetworkInterface>
#include "mainwindow.h"
#include "clientsocket.h"
namespace Ui {
class MainWindow2;
}

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = 0);
    ~MainWindow2();
    QWebSocketServer *m_pWebSocketServer;
    database d;
    QString Connectors[3]={"1","2","3"};
    MainWindow *w=new MainWindow();
    clientsocket *tcps = new clientsocket();
    QTimer soft_reset_timer;
    QDateTime issueDate = QDateTime::currentDateTime();
    QString CurrentDate=issueDate.toUTC().toString();
private slots:
    void softreset();

    void on_pushButton_Cancel_Reservation_clicked();

    void on_pushButton_Change_Availability_clicked();

    void on_pushButton_Change_Configuration_clicked();

    void on_pushButton_Clear_Cache_clicked();

//    void on_pushButton_Clear_Charging_Profile_clicked();

    void on_pushButton_DataTransfer_clicked();

//    void on_pushButton_Get_Composite_Schedule_clicked();

    void on_pushButton_Get_Configuration_clicked();

    void on_pushButton_Get_Diagnostics_clicked();

    void on_pushButton_Get_Local_List_Version_clicked();

    void on_pushButton_Remote_Start_Transaction_clicked();

    void on_pushButton_Remote_Stop_Transaction_clicked();

    void on_pushButton_Reserve_Now_clicked();

    void on_pushButton_Reset_clicked();

    void on_pushButton_Send_Local_List_clicked();

//    void on_pushButton_Set_Charging_Profile_clicked();

    void on_pushButton_Trigger_Message_clicked();

    void on_pushButton_Unlock_Connector_clicked();

    void on_pushButton_Update_Firmware_clicked();

    void on_pushButton_StartChargePoint_clicked();

    void on_pushButton_StopChargePoint_clicked();

public:
    QString msg_Id,uniq_Id,Payload,result,PayloadHead,time_text,message,host,cp_name;
    void Sub2();
    void onNewConnection();
    void processTextMessage(QString message);
    void socketDisconnected();


    void CancelReservation();
    void ChangeAvailability();
    void ChangeConfiguration();
    void ClearCache();
//    void ClearChargingProfile();
    void DataTransfer();
//    void GetCompositeSchedule();
    void GetConfiguration();
    void GetDiagnostics(QString FILENAME);
    void GetLocalListVersion();
    void RemoteStartTransaction();
    void RemoteStopTransaction();
    void ReserveNow();
    void Reset();
    void SendLocalList();
//    void SetChargingProfile();
    void TriggerMessage();
    void UnlockConnector();
    void UpdateFirmware();
    QString ExpiryDate,Status,IdTag,Interval,SerialNumber,ClientId,
    Model,ChargePointSerialNumber,Vendor,FirmwareVersion,ICCID,IMSI,MeterSerialNumber,MeterType,Reason,MeterStop,
    CurrentTime,TimeStamp,TransactionId,MeterValue,ConnectorId,Value,Context,Format,Measured,Phase,Location,Unit,
    MeterStart,ReservationId,ParentIdTag,Transaction,UpdateType,FILENAME,Duration,ExpiryTime,ReservedTime,
    StackLevel,UnitType,MinChargingRate,StartPeriod,Limit,NumberPhases,Type,Key,NewValue,PurposeType,VendorId,MessageId,
    Data,ChargingUnitType,Retries,RetryInterval,StartTime,StopTime,RequestMessage,RetrieveDate,LocalListVersion;
 int chngavail,availability,res,mON;
private:
    QList<QWebSocket *> m_clients;
    bool m_debug;
    Ui::MainWindow2 *ui;
};

#endif // MAINWINDOW2_H
