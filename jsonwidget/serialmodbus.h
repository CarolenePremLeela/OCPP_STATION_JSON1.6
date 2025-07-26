#ifndef SERIALMODBUS_H
#define SERIALMODBUS_H

#include <QWidget>
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
class serialmodbus;
}

class serialmodbus : public QWidget
{
    Q_OBJECT

public:
    explicit serialmodbus(QWidget *parent = 0);
    ~serialmodbus();
    double last_eb_total[50];
    QSerialPort serial;
    QTimer timer;
    QByteArray temp;
    int size;
    float Voltage,Current,Frequency,Energy;
    QString voltagevalue,freqvalue,energy,currentvalue;
    void convert_to_float();
    void writeRequestVoltage(int slave_id);
    void writeRequestCurrent(int slave_id);
    void writeRequestFreq(int slave_id);
    void writeRequestEnergy(int slave_id);

    int readResponseVoltage(int time, float *unit, int id);
    int readResponseCurrent(int time, float *unit, int id);
    int readResponseFreq(int time, float *unit, int id);
    int readResponseEnergy(int time, float *unit, int id);


    unsigned int CRC16(const char *nData, int wLength);
    void start();
    int status;
private:
    Ui::serialmodbus *ui;

};

#endif // SERIALMODBUS_H
