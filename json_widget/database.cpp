#include "database.h"
#include<QMessageBox>
#include<QDebug>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
database::database(QObject *parent) : QObject(parent)
{
   createConnection();
}
database::~database()
{

}
bool database::createConnection()
{
   QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
//   db.setHostName("192.168.1.32");
//   db.setDatabaseName("fca");
//   db.setUserName("root");
//   db.setPassword("");
//   db.setHostName("103.83.81.25");
//   db.setDatabaseName("fca");
//   db.setUserName("ocppuser");
//   db.setPassword("IafZ0$jER");
   db.setHostName("103.83.81.25");
   db.setDatabaseName("fca");
   db.setUserName("ocpp");
   db.setPassword("4ZEcka&JI");

   if (!db.open())
   {
       std::cerr<<"error while opening database.."<<std::endl;

       QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
       return false;
   }else
   {std::cerr<<" opening database.."<<std::endl;
   return true;
   }
}
void database::get_connectorids(QString *ConnectorId, QString ClientIp)
{
   QString ConnectorIds;
   QSqlQuery query;
   query.prepare("select `connector_id` from `connectorid_details` where `client_id`=(SELECT `client_id` FROM `client_table` WHERE `client_ip`=:ClientIp)");
   query.bindValue(":ClientIp",ClientIp);
   query.exec();
   while(query.next())
   {
       ConnectorIds=ConnectorIds+"/"+query.value(0).toString();
   }
   *ConnectorId=ConnectorIds;
}
void database::get_client_id(QString *ClientId, QString ClientIp)
{
    QSqlQuery query;
    query.prepare("SELECT `client_id` FROM `client_table` WHERE `client_ip`=:ClientIp");
    query.bindValue(":ClientIp",ClientIp);
      query.exec();
      while(query.next())
      {
           *ClientId  =query.value(0).toString();
      }
}
void database::get_bitset(int *boot,int *diag,int *firm, int *heart,int *statusnoti,int ClientId)
{
   QSqlQuery query;
query.prepare("SELECT `boot`,`diagnostics`,`firmware`,`heartbeat`,`statusnoti` FROM `bitset` WHERE client_id=:ClientId");
query.bindValue(":ClientId",ClientId);
query.exec();
while(query.next())
{
     *boot =query.value(0).toInt();
     *diag =query.value(1).toInt();
     *firm =query.value(2).toInt();
     *heart =query.value(3).toInt();
     *statusnoti =query.value(4).toInt();
}
}
void database::get_client_changeavailability(int *boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT `change_availability` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId");
    query.bindValue(":ConnectorId",ConnectorId);
    query.exec();
    while(query.next())
    {
        *boot=query.value(0).toInt();
    }
}
void database::get_client_remotestartstatus(int *boot, QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT   `Start` FROM `connectorid_details` WHERE `connector_id` =:ConnectorId ");
      query.bindValue(":ConnectorId",ConnectorId);
      query.exec();
      while(query.next())
      {
           *boot  =query.value(0).toInt();
      }
}
void database::get_client_remotestopstatus(int *boot, QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT   `Stop` FROM `connectorid_details` WHERE `connector_id` =:ConnectorId");
      query.bindValue(":ConnectorId",ConnectorId);
      query.exec();
      while(query.next())
      {
           *boot  =query.value(0).toInt();
      }
}
void database::get_client_triggermetervalue(int *boot, QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT `metervalue` FROM `connectorid_details` WHERE connector_id=:ConnectorId");
      query.bindValue(":ConnectorId",ConnectorId);
      query.exec();
      while(query.next())
      {
           *boot  =query.value(0).toInt();
      }
}
void database::get_client_meter_ONOFF_status(int *boot, QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("SELECT `meter_status` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId");
      query.bindValue(":ConnectorId",ConnectorId);
      query.exec();
      while(query.next())
      {
           *boot  =query.value(0).toInt();
      }
}
void database::reserveNow_Status(int *boot, QString ConnectorId)
{
   QSqlQuery query; 
   query.prepare("SELECT  `reserve_status` FROM `connectorid_details` WHERE `connector_id` =:ConnectorId ");
   query.bindValue(":ConnectorId",ConnectorId);
   query.exec();
   while(query.next())
   {
        *boot  =query.value(0).toInt();
   }
}
void database::o_client_boot(int boot,QString ClientId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `boot`=:boot where `client_id`=:ClientId");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::o_client_diag(int boot,QString ClientId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `diagnostics`=:boot where `client_id`=:ClientId");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::o_client_firm(int boot,QString ClientId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `firmware`=:boot where `client_id`=:ClientId");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::o_client_heartbeat(int boot,QString ClientId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `heartbeat`=:boot where `client_id`=:ClientId");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::update_client_metervalue(int boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `connectorid_details` SET `metervalue`=:boot where `connector_id`=:ConnectorId");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::cp_getReservation(QString ConnectorId,QString *IdTag,QString *ReservationId)
{
   QSqlQuery query;
   query.prepare("select `idtag`,`reservation_id` from `reservation_backup` where `connector_id`=:ConnectorId AND `cancel_bit`='0'");
   query.bindValue(":ConnectorId",ConnectorId);
   query.exec();
   while(query.next())
   {
       *IdTag         =query.value(0).toByteArray();
       *ReservationId =query.value(1).toByteArray();
   }

}
void database::get_parentidtag_frm_idtag(QString *ParentIdTag, QString IdTag)
{
    QSqlQuery query;
    query.prepare("SELECT `parent_idtag` FROM `user_details` WHERE `idtag` =:IdTag");
    query.bindValue(":IdTag",IdTag);
    query.exec();
    while(query.next())
    {
        *ParentIdTag=query.value(0).toString();
    }
}
void database::get_StartTransaction(QString ConnectorId,QString *IdTag,QString *ReservationId)
{
   QSqlQuery query;
   query.prepare("select `idtag`,`reservation_id` from `transaction_table` where `connector_id`=:ConnectorId ORDER BY `id` DESC LIMIT 1");
   query.bindValue(":ConnectorId",ConnectorId);
   query.exec();
   while(query.next())
   {
       *IdTag         =query.value(0).toByteArray();
       *ReservationId =query.value(1).toByteArray();
   }
}
void database::get_TransactionId(QString *TransactionId, QString ConnectorId)
{
   QSqlQuery query;
   query.prepare("SELECT   `transaction_id` FROM `transaction_table` WHERE `connector_id` =:ConnectorId ORDER BY `id` DESC LIMIT 1");
   query.bindValue(":ConnectorId",ConnectorId);
   query.exec();
   while(query.next())
   {
        *TransactionId  =query.value(0).toByteArray();
   }

}
void database::update_RemoteStart(int start,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE `connectorid_details` SET `Start`=:Start where `connector_id`=:ConnectorId");
    query.bindValue(":Start",start);
    query.bindValue(":ConnectorId",ConnectorId);
    query.exec();
}
void database::update_RemoteStop(int stop,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE `connectorid_details` SET `Stop`=:Stop where `connector_id`=:ConnectorId");
    query.bindValue(":Stop",stop);
    query.bindValue(":ConnectorId",ConnectorId);
    query.exec();
}
void database::o_client_statusnoti(int boot,QString ClientId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `statusnoti`=:boot where `client_id`=:ClientId");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":boot",boot);
    query.exec();
}




void database::update_reservestatus(int reserve_status,QString ReservationId)
{
    QSqlQuery query;
    query.prepare("UPDATE `connectorid_details` SET `reserve_status`=:reserve_status where `connector_id`=(SELECT `connector_id` FROM `reservation_backup` WHERE `reservation_id`=:ReservationId)");
    query.bindValue(":reserve_status",reserve_status);
    query.bindValue(":ReservationId",ReservationId);
    query.exec();
}
void database::dropReservationId(QString ReservationId)
{
    QSqlQuery query;
    query.prepare("UPDATE `reservation_backup` SET `cancel_bit`='1' WHERE `reservation_id`=:ReservationId");
    query.bindValue(":ReservationId",ReservationId);
    query.exec();
}
void database::update_client_changeavailability(int boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `connectorid_details` SET `change_availability`=:boot where `connector_id`=:ConnectorId");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::Insert_RemoteStartTransaction(QString ConnectorId, QString IdTag,QString TransactionId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO `transaction_table` (`connector_id`,`idtag`,`transaction_id`) VALUES (:ConnectorId,:IdTag,:TransactionId)");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":IdTag",IdTag);
    query.bindValue(":TransactionId",TransactionId);
    qDebug() << query.isValid();
    query.exec();
}
void database::get_conid_frm_tranid(QString *ConnectorId, QString TransactionId)
{
    QSqlQuery query;
    query.prepare("SELECT `connector_id` FROM `transaction_table` WHERE `transaction_id` =:TransactionId ORDER BY `id` DESC LIMIT 1");
    query.bindValue(":TransactionId",TransactionId);
    query.exec();
    while(query.next())
    {
        *ConnectorId=query.value(0).toString();
    }
}
void database::reservebackup(QString ReservationId,QString UserId,QString ConnectorId,QString TimeNow,QString ReservedTime, QString ExpiryDate,QString Duration)
{
    QSqlQuery query;
    query.prepare("INSERT INTO `reservation_backup` (`reservation_id`,`idtag`,`connector_id`,`time_now`,`reserved_time`,`expiry_date`,`duration(hrs)`,`cancel_bit`) VALUES(:ReservationId,:UserId,:ConnectorId,:TimeNow,:ReservedTime,:ExpiryDate,:Duration,'0')");
    query.bindValue(":ReservationId",ReservationId);
    query.bindValue(":UserId",UserId);
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":TimeNow",TimeNow);
    query.bindValue(":ReservedTime",ReservedTime);
    query.bindValue(":ExpiryDate",ExpiryDate);
    query.bindValue(":Duration",Duration);
    query.exec();
}
void database::update_client_boot(int boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `boot`=:boot where `client_id`=(SELECT `client_id` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId )");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::update_client_diag(int boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `diagnostics`=:boot where `client_id`=(SELECT `client_id` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId )");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::update_client_firm(int boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `firmware`=:boot where `client_id`=(SELECT `client_id` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId )");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}
void database::update_client_heartbeat(int boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `heartbeat`=:boot where `client_id`=(SELECT `client_id` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId )");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}

void database::update_client_statusnoti(int boot,QString ConnectorId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `bitset` SET `statusnoti`=:boot where `client_id`=(SELECT `client_id` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId )");
    query.bindValue(":ConnectorId",ConnectorId);
    query.bindValue(":boot",boot);
    query.exec();
}



void database::get_chargertype_for_starttransaction(QString ClientId,QString *ConnectorIdfor7,QString *ConnectorIdfor8)
{
   QSqlQuery query;
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=7 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor7         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=8 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor8         =query.value(0).toByteArray();
   }
}

void database::get_chargertype(QString ClientId,QString *ConnectorIdfor7,QString *ConnectorIdfor8,QString *ConnectorIdfor1,QString *ConnectorIdfor2,QString *ConnectorIdfor3,QString *ConnectorIdfor4,QString *ConnectorIdfor5,QString *ConnectorIdfor6)
{
   QSqlQuery query;
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=7 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor7         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=8 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor8         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=1 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor1         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=2 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor2         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=3 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor3         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=4 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor4         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=5 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor5         =query.value(0).toByteArray();
   }
   query.prepare("SELECT `connector_id` FROM `connectorid_details` WHERE `charger_id`=6 AND `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *ConnectorIdfor6         =query.value(0).toByteArray();
   }
}
void database::get_interval(QString ClientId,int *Interval)
{
   QSqlQuery query;
   query.prepare("SELECT `interval(hr)` FROM `client_table` WHERE `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *Interval         =query.value(0).toInt();
   }
}
void database::get_NoofConnectors_usingclienttable(int *No_of_Connectors, QString ClientId)
{
   QSqlQuery query;
   query.prepare("SELECT `no_of_connectors` FROM `client_table` WHERE `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
        *No_of_Connectors  =query.value(0).toInt();
   }
}
void database::get_chargertype_using_connectorid(QString *ChargerType,QString *ChargerPower,QString *ChargerCurrent,QString *SocketType, QString ConnectorId)
{
   QSqlQuery query;
   query.prepare("SELECT `AC/DC`,`charger_power`,`charger_current`,`socket_on_EVside` FROM `charger_type_table` WHERE `charger_id`=(SELECT `charger_id` FROM `connectorid_details` WHERE `connector_id`=:ConnectorId)");
   query.bindValue(":ConnectorId",ConnectorId);
   query.exec();
   while(query.next())
   {
        *ChargerType  =query.value(0).toString();
        *ChargerPower  =query.value(1).toString();
        *ChargerCurrent  =query.value(2).toString();
        *SocketType  =query.value(3).toString();
   }
}
void database::registration_insert(QString ClientId,QString ClientName,QString CompanyName,QString ClientMailId, QString ClientAddress,QString ClientPincode,QString ClientMobileNo,QString ClientLandLineNo,QString ClientPassword)
{
    QSqlQuery query;
    query.prepare("UPDATE `client_table` SET `client_name`=:ClientName,`client_mailid`=:ClientMailId,`client_address`=:ClientAddress,`client_pincode`=:ClientPincode,`client_mobile_no`=:ClientMobileNo,`client_landline_no`=:ClientLandLineNo,`client_password`=:ClientPassword  WHERE `client_id`=:ClientId");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":ClientName",ClientName);
    query.bindValue(":CompanyName",CompanyName);
    query.bindValue(":ClientMailId",ClientMailId);
    query.bindValue(":ClientAddress",ClientAddress);
    query.bindValue(":ClientPincode",ClientPincode);
    query.bindValue(":ClientMobileNo",ClientMobileNo);
    query.bindValue(":ClientLandLineNo",ClientLandLineNo);
    query.bindValue(":ClientPassword",ClientPassword);
    query.exec();
}
void database::get_clientidforregistration(QString *ClientId, QString MeterSerialNo)
{
   QSqlQuery query;
   query.prepare("SELECT `client_id` FROM `client_table` WHERE `serial_no`=:MeterSerialNo");
   query.bindValue(":MeterSerialNo",MeterSerialNo);
   query.exec();
   while(query.next())
   {
       *ClientId         =query.value(0).toByteArray();
   }
}
void database::get_passwordforlogin(QString *password, QString ClientId)
{
   QSqlQuery query;
   query.prepare("SELECT `client_password` FROM `client_table` WHERE `client_id`=:ClientId");
   query.bindValue(":ClientId",ClientId);
   query.exec();
   while(query.next())
   {
       *password        =query.value(0).toByteArray();
   }
}
void database::get_passwordformanagerlogin(QString *password, QString ManagerId)
{
   QSqlQuery query;
   query.prepare("SELECT `manager_password` FROM `manager_table` WHERE `manager_id`=:ManagerId");
   query.bindValue(":ManagerId",ManagerId);
   query.exec();
   while(query.next())
   {
       *password        =query.value(0).toByteArray();
   }
}
void database::update_clientip(QString ClientIP,QString ClientId)
{
    QSqlQuery query;
    query.prepare("UPDATE  `client_table` SET `client_ip`=:ClientIP where `client_id`=:ClientId");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":ClientIP",ClientIP);
    query.exec();
}
void database::insert_Rate_Settings(QString ClientId,QString ACDC,QString RatePeriod1_Start,QString RatePeriod1_Stop,QString RatePeriod2_Start,QString RatePeriod2_Stop,QString RatePeriod3_Start,QString RatePeriod3_Stop,QString RatePeriod4_Start,QString RatePeriod4_Stop,QString Rate1,QString Rate2,QString Rate3,QString Rate4,QString timer_stamp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO rate_settings(client_id,ACDC,RatePeriod1_Start,RatePeriod1_Stop,RatePeriod2_Start,RatePeriod2_Stop,RatePeriod3_Start,RatePeriod3_Stop,RatePeriod4_Start,RatePeriod4_Stop,Rate1,Rate2,Rate3,Rate4,timer_stamp) VALUES (:ClientId,:ACDC,:ratestart1,:ratestop1,:ratestart2,:ratestop2,:ratestart3,:ratestop3,:ratestart4,:ratestop4,:rate1,:rate2,:rate3,:rate4,:timer_stamp)");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":ACDC",ACDC);
    query.bindValue(":ratestart1",RatePeriod1_Start);
    query.bindValue(":ratestop1",RatePeriod1_Stop);
    query.bindValue(":ratestart2",RatePeriod2_Start);
    query.bindValue(":ratestop2",RatePeriod2_Stop);
    query.bindValue(":ratestart3",RatePeriod3_Start);
    query.bindValue(":ratestop3",RatePeriod3_Stop);
    query.bindValue(":ratestart4",RatePeriod4_Start);
    query.bindValue(":ratestop4",RatePeriod4_Stop);
    query.bindValue(":rate1",Rate1);
    query.bindValue(":rate2",Rate2);
    query.bindValue(":rate3",Rate3);
    query.bindValue(":rate4",Rate4);
    query.bindValue(":timer_stamp",timer_stamp);
    query.exec();
}
void database::get_starttime_usingconnectorId(QString *metervalue,QString *StartTime, QString ConnectorId)
{
   QSqlQuery query;
   query.prepare("SELECT `timestamp`,`value` FROM `transaction_table` WHERE `meter_start`='1' AND `connector_id`=:ConnectorId ORDER BY `id` DESC LIMIT 1");
   query.bindValue(":ConnectorId",ConnectorId);
   query.exec();
   while(query.next())
   {
       *StartTime    =query.value(0).toByteArray();
       *metervalue   =query.value(1).toByteArray();
   }
}
void database::insert_Trading(QString IdTag,QString Start_time,QString Stop_time,QString StartUnit,QString StopUnit,QString Charge_KWH,QString Fare_KWH,QString Total_fare,QString Service_fare)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Trading(idtag,start_time,stop_time,start_unit,stop_unit,charge_kwh,fare_kwh,total_fare,service_fare) VALUES (:IdTag,:Starttime,:Stoptime,:StartUnit,:StopUnit,:ChargeKWH,:FareKWH,:Totalfare,:Servicefare)");
    query.bindValue(":IdTag",IdTag);
    query.bindValue(":Starttime",Start_time);
    query.bindValue(":Stoptime",Stop_time);
    query.bindValue(":StartUnit",StartUnit);
    query.bindValue(":StopUnit",StopUnit);
    query.bindValue(":ChargeKWH",Charge_KWH);
    query.bindValue(":FareKWH",Fare_KWH);
    query.bindValue(":Totalfare",Total_fare);
    query.bindValue(":Servicefare",Service_fare);
    query.exec();
}
void database::insert_faultreport(QString Occurrence,QString FaultCode,QString FaultDetails)
{
    QSqlQuery query;
    query.prepare("INSERT INTO `fault_table`( `occurence`, `fault_code`, `fault_details`) VALUES (:Occurrence,:FaultCode,:FaultDetails)");
    query.bindValue(":Occurrence",Occurrence);
    query.bindValue(":FaultCode",FaultCode);
    query.bindValue(":FaultDetails",FaultDetails);
    query.exec();
}
void database::get_idtag_QR(QString OTP,QString UserName,QString *IdTag)
{
    QSqlQuery query;
    query.prepare("SELECT `idtag` FROM `user_details` WHERE `user_name`=:UserName AND `OTP`=:OTP");
    query.bindValue(":UserName",UserName);
    query.bindValue(":OTP",OTP);
    query.exec();
    while(query.next())
    {
        *IdTag    =query.value(0).toByteArray();
    }
}
void database::manager_registration_insert(QString ClientId,QString ManagerId,QString ManagerName,QString CompanyName,QString ManagerMailId, QString ManagerAddress,QString ManagerMobileNo,QString ManagerLandLineNo,QString ManagerPassword)
{
    QSqlQuery query;
    query.prepare("INSERT INTO `manager_table` (`manager_id`,`manager_name`,`manager_company_name`,`manager_mailid`,`manager_address`,`manager_mobile_no`,`manager_landline_no`,`manager_password`,`client_id`) VALUES (:ManagerId,:ManagerName,:CompanyName,:ManagerMailId,:ManagerAddress,:ManagerMobileNo,:ManagerLandLineNo,:ManagerPassword,:ClientId)");
    query.bindValue(":ClientId",ClientId);
    query.bindValue(":ManagerId",ManagerId);
    query.bindValue(":ManagerName",ManagerName);
    query.bindValue(":CompanyName",CompanyName);
    query.bindValue(":ManagerMailId",ManagerMailId);
    query.bindValue(":ManagerAddress",ManagerAddress);
    query.bindValue(":ManagerMobileNo",ManagerMobileNo);
    query.bindValue(":ManagerLandLineNo",ManagerLandLineNo);
    query.bindValue(":ManagerPassword",ManagerPassword);
    query.exec();
}
void database::get_logincredentials(QString *credentials, QString type)
{
   QSqlQuery query;
   query.prepare("SELECT  `credentials` FROM `login_credentials` WHERE `type_of_user`=:type");
   query.bindValue(":type",type);
   query.exec();
   while(query.next())
   {
       *credentials        =query.value(0).toByteArray();
   }
}
void database::get_clientid_using_IP(QString *ClientID, QString ClientIP)
{
   QSqlQuery query;
   query.prepare("SELECT  `client_id` FROM `client_table` WHERE `client_ip`=:ClientIP");
   query.bindValue(":ClientIP",ClientIP);
   query.exec();
   while(query.next())
   {
       *ClientID        =query.value(0).toByteArray();
   }
}
void database::get_managerid_using_clientid(QString *ManagerID, QString ClientID)
{
   QSqlQuery query;
   query.prepare("SELECT  `manager_id` FROM `manager_table` WHERE `client_id`=:ClientID");
   query.bindValue(":ClientID",ClientID);
   query.exec();
   while(query.next())
   {
       *ManagerID        =query.value(0).toByteArray();
   }
}
void database::delete_OTP_QR(QString UserName,QString IdTag)
{
    QSqlQuery query;
    query.prepare("UPDATE `user_details` SET `OTP`='' WHERE `user_name`=:UserName AND `idtag`=:IdTag");
    query.bindValue(":UserName",UserName);
    query.bindValue(":IdTag",IdTag);
    query.exec();
}
