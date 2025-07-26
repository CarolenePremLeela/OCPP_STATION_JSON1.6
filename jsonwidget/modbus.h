#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<string>
#include<QtSerialPort/QSerialPort>
#include<QTimer>

#include <QString>
#include <QMainWindow>
union mytypes_t                             // union to change ieee754 to float point
{						  					// while ask float f it will return float  format of the ieee754 unsigned char[] value because in union all variables share the same memory
    unsigned char i[4];
    float f;
};

union mytypes_l                             // union to change to long point
{
    unsigned char i[4];
    long int f;
};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double last_eb_total[50];
    QSerialPort serial;
    QTimer timer;
    QByteArray temp;
    void convert_to_float();
    void writeRequestVoltage(int slave_id);
    void writeRequestFreq(int slave_id);
    void rhsRequest(int slave_id);
    void Get_fulldetails_request(int slave_id);
    int readResponseVoltage(int time, float *unit, int id);
    int readResponseFreq(int time, float *unit, int id);

    int readrhsResponse(int time, long int *RHS, QString com, int id);
    void convertrhs(long RHS, QString *String_RHS);
    int readEnergyResponse(int time, float *AVG_LOAD, float *L1, float *L2, float *L3, QString com, int id);
    int read_fulldetail_Response(int time, float *A, float *Al, float *A2, float *A3, float *VLL, float *V12, float *V23, float *V31, float *W, float *W1, float *W2, float *W3, float *PF, float *F, float *WHU, QString com, int id);
    void writeEnergyRequest(int slave_id);
    unsigned int CRC16(const char *nData, int wLength);
    void start();
    int status;
private slots:
    void myfunction();

private:
    Ui::MainWindow *ui;
    QTimer *timer1;


};

#endif // MAINWINDOW_H
