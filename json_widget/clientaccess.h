#ifndef clientaccess_H
#define clientaccess_H

#include <QMainWindow>
#include <QTimer>
#include "mainwindow2.h"
#include "serialmodbus.h"
#include "dialog.h"
namespace Ui {
class clientaccess;
}

class clientaccess : public QMainWindow
{
    Q_OBJECT

public:
    explicit clientaccess(QWidget *parent = 0);
    ~clientaccess();
    MainWindow2 *w2=new MainWindow2();
    serialmodbus serial;
    Dialog *dial;
    int count=1,netcount=1;
    QString ClientId,RatePeriod1_Start,RatePeriod1_Stop,RatePeriod2_Start,RatePeriod2_Stop,RatePeriod3_Start,RatePeriod3_Stop,RatePeriod4_Start,RatePeriod4_Stop,Rate1,Rate2,Rate3,Rate4;
    void get_Idtag(QString IdTag);

    void on_pushButton_failure_clicked(QString GMT,QString Fault,QString Fault_Code);

private slots:
    void myfunction();
    void on_toolButton_SystemSettings_clicked();

    void on_toolButton_Costset_clicked();

    void on_toolButton_IPSet_clicked();

    void on_toolButton_OtherSet_clicked();

    void on_toolButton_fault_clicked();

    void on_toolButton_Trading_clicked();

    void on_toolButton_Manage_clicked();

    void on_toolButton_Settings_clicked();

    void on_toolButton_Info_clicked();

    void on_toolButton_Start_Con_1_clicked();

    void on_toolButton_Stop_Con_1_clicked();

    void on_toolButton_Start_Con_2_clicked();

    void on_toolButton_Stop_Con_2_clicked();

    void on_toolButton_Start_Con_3_clicked();

    void on_toolButton_Stop_Con_3_clicked();

    void on_toolButton_gotoregister_clicked();

    void on_toolButton_gotologin_clicked();

    void on_toolButton_register_submit_clicked();

    void on_toolButton_register_cancel_clicked();

    void on_toolButton_Client_Login_clicked();

    void on_toolButton_Client_Login_Cancel_clicked();

    void on_toolButton_Confirm_IPConfig_clicked();
    
    void on_toolButton_Cancel_IPConfig_clicked();

    void on_toolButton_Client_Login_withoutreg_clicked();

    void on_toolButton_Client_Login_Cancel_withoutreg_clicked();

    void on_toolButton_prev_clicked();

    void on_toolButton_next_clicked();

    void on_toolButton_AC_Rate_clicked();

    void on_toolButton_DC_Rate_clicked();

    void on_toolButton_DC_Enter_clicked();

    void on_toolButton_AC_Enter_clicked();

    void on_toolButton_IdTag_Confirm_clicked();

    void trading();

    void on_toolButton_Trading_Return_clicked();

    void on_toolButton_Fault_Export_clicked();

    void on_toolButton_fault_clear_clicked();

    void on_toolButton_Fault_Return_clicked();

    void on_toolButton_Trading_Export_clicked();

    void on_toolButton_IdTag_Cancel_clicked();

    void on_toolButton_Confirm_IPConfig_r_clicked();

    void on_toolButton_Cancel_IPConfig_r_clicked();

    void on_toolButton_DC_Enter_r_clicked();

    void on_toolButton_AC_Enter_r_clicked();

    void on_toolButton_return_commonPage_clicked();

    void on_toolButton_Keyboard_clicked();

    void on_toolButton_exit_clicked();

    void on_toolButton_Trading_Clear_clicked();

    void on_toolButton_QRScan_clicked();

    void on_toolButton_RFID_Scan_clicked();

    void on_toolButton_QR_Confirm_clicked();

    void on_toolButton_QR_Cancel_clicked();

    void on_toolButton_register_submit_manage_clicked();

    void on_toolButton_register_cancel_manage_clicked();

    void on_toolButton_Client_Login_Settings_clicked();

    void on_toolButton_Client_Login_Cancel_Settings_clicked();

    void on_toolButton_info_bottom_clicked();

    void on_toolButton_AC_Rate_r_clicked();

    void on_toolButton_DC_Rate_r_clicked();

private:
    Ui::clientaccess *ui;
    QTimer *timer;
};

#endif // clientaccess_H
