#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include<iostream>

#include<QtSql>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlDriver>
#include<QtSql/QSqlQuery>


class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = 0);
    ~database();
    bool createConnection();
    void get_connectorids(QString *ConnectorId, QString ClientIp);
    void get_client_id(QString *ClientId, QString ClientIp);
    
    void get_bitset(int *boot, int *diag, int *firm, int *heart, int *statusnoti, int ClientId);
    void get_client_changeavailability(int *boot, QString ConnectorId);
    void get_client_remotestartstatus(int *boot,QString ConnectorId);
    void get_client_remotestopstatus(int *boot, QString ConnectorId);
    void get_client_triggermetervalue(int *boot, QString ConnectorId);
    void get_client_meter_ONOFF_status(int *boot, QString ConnectorId);
    void reserveNow_Status(int *boot, QString ConnectorId);
    
    
    void o_client_boot(int boot,QString ClientId);
    void o_client_diag(int boot,QString ClientId);
    void o_client_firm(int boot,QString ClientId);
    void o_client_heartbeat(int boot,QString ClientId);
    void update_client_metervalue(int boot,QString ConnectorId);
    void cp_getReservation(QString ConnectorId, QString *IdTag, QString *ReservationId);
    void get_parentidtag_frm_idtag(QString *ParentIdTag, QString IdTag);
    void get_StartTransaction(QString ConnectorId,QString *IdTag,QString *ReservationId);
    void get_TransactionId(QString *TransactionId, QString ConnectorId);
    void update_RemoteStart(int start,QString ConnectorId);
    void update_RemoteStop(int stop,QString ConnectorId);
    void o_client_statusnoti(int boot,QString ClientId);


    void update_reservestatus(int reserve_status, QString ReservationId);
    void dropReservationId(QString ReservationId);
    void update_client_changeavailability(int boot,QString ConnectorId);
    void Insert_RemoteStartTransaction(QString ConnectorId, QString IdTag,QString TransactionId);
    void get_conid_frm_tranid(QString *ConnectorId, QString TransactionId);
    void reservebackup(QString ReservationId, QString UserId, QString ConnectorId, QString TimeNow, QString ReservedTime, QString ExpiryDate, QString Duration);
    void update_client_boot(int boot,QString ConnectorId);
    void update_client_diag(int boot,QString ConnectorId);
    void update_client_firm(int boot,QString ConnectorId);
    void update_client_heartbeat(int boot,QString ConnectorId);
    void update_client_statusnoti(int boot, QString ConnectorId);

    void get_chargertype_for_starttransaction(QString ClientId,QString *ConnectorIdfor7,QString *ConnectorIdfor8);
    void get_chargertype(QString ClientId, QString *ConnectorIdfor7, QString *ConnectorIdfor8, QString *ConnectorIdfor1, QString *ConnectorIdfor2, QString *ConnectorIdfor3, QString *ConnectorIdfor4, QString *ConnectorIdfor5, QString *ConnectorIdfor6);
    void get_interval(QString ClientId,int *Interval);
    void get_NoofConnectors_usingclienttable(int *No_of_Connectors, QString ClientId);

    void registration_insert(QString ClientId, QString ClientName, QString CompanyName, QString ClientMailId, QString ClientAddress,QString ClientPincode, QString ClientMobileNo, QString ClientLandLineNo, QString ClientPassword);
    void get_clientidforregistration(QString *ClientId, QString MeterSerialNo);
    void get_passwordforlogin(QString *password, QString ClientId);
    void get_passwordformanagerlogin(QString *password, QString ManagerId);
    void update_clientip(QString ClientIP,QString ClientId);
    void insert_Rate_Settings(QString ClientId,QString ACDC,QString RatePeriod1_Start,QString RatePeriod1_Stop,QString RatePeriod2_Start,QString RatePeriod2_Stop,QString RatePeriod3_Start,QString RatePeriod3_Stop,QString RatePeriod4_Start,QString RatePeriod4_Stop,QString Rate1,QString Rate2,QString Rate3,QString Rate4,QString TimeStamp);
    void get_starttime_usingconnectorId(QString *metervalue, QString *StartTime, QString ConnectorId);
    void insert_Trading(QString IdTag, QString Start_time, QString Stop_time, QString StartUnit, QString StopUnit, QString Charge_KWH, QString Fare_KWH, QString Total_fare, QString Service_fare);
    void insert_faultreport(QString Occurrence,QString FaultCode,QString FaultDetails);
    void get_idtag_QR(QString OTP,QString UserName,QString *IdTag);
    void manager_registration_insert(QString ClientId,QString ManagerId,QString ManagerName,QString CompanyName,QString ManagerMailId, QString ManagerAddress,QString ManagerMobileNo,QString ManagerLandLineNo,QString ManagerPassword);
    void get_logincredentials(QString *credentials, QString type);
    void get_clientid_using_IP(QString *ClientID, QString ClientIP);
    void get_managerid_using_clientid(QString *ManagerID, QString ClientID);
    void delete_OTP_QR(QString UserName,QString IdTag);

    void get_chargertype_using_connectorid(QString *ChargerType,QString *ChargerPower,QString *ChargerCurrent,QString *SocketType, QString ConnectorId);
signals:

public slots:
};

#endif // DATABASE_H
