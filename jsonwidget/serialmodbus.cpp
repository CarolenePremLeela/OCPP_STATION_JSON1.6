#include "serialmodbus.h"
#include "ui_serialmodbus.h"
#include <QDebug>
serialmodbus::serialmodbus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serialmodbus)
{
    ui->setupUi(this);
}

serialmodbus::~serialmodbus()
{
    delete ui;
}

void serialmodbus::writeRequestVoltage(int slave_id)
{

    char req[]={(char)(0x01),(char)(0x04),(char)(0x00),(char)(0x00),(char)(0x00),(char)(0x02),(char)(0x86),(char)(0x06)};//(slave_id,Functions,force_multiple_coils,No.of points Hi,No.of points Lo,Error Check (LRC or CRC))
//0F=Return Slave No Response Count

    unsigned int crc=0x00;

    req[0]=slave_id;

    crc=CRC16(req,6);
    req[6]=0x00;
    req[6]=crc&0x00FF;
    crc=crc>>8;
    req[7]=crc&0x00FF;

    if(!serial.write(req,8))
    {
        qDebug()<<"error while writing port";
        return;
    }

 }
void serialmodbus::writeRequestCurrent(int slave_id)
{

    char req[]={(char)(0x01),(char)(0x04),(char)(0x00),(char)(0x06),(char)(0x00),(char)(0x02),(char)(0x86),(char)(0x06)};//(slave_id,Functions,force_multiple_coils,No.of points Hi,No.of points Lo,Error Check (LRC or CRC))
//0F=Return Slave No Response Count

    unsigned int crc=0x00;

    req[0]=slave_id;

    crc=CRC16(req,6);
    req[6]=0x00;
    req[6]=crc&0x00FF;
    crc=crc>>8;
    req[7]=crc&0x00FF;

    if(!serial.write(req,8))
    {
        qDebug()<<"error while writing port";
        return;
    }

 }
void serialmodbus::writeRequestFreq(int slave_id)
{

    char req[]={(char)(0x01),(char)(0x04),(char)(0x00),(char)(0x18),(char)(0x00),(char)(0x02),(char)(0x86),(char)(0x06)};//(slave_id,Functions,force_multiple_coils,No.of points Hi,No.of points Lo,Error Check (LRC or CRC))
//0F=Return Slave No Response Count

    unsigned int crc=0x00;

    req[0]=slave_id;

    crc=CRC16(req,6);
    req[6]=0x00;
    req[6]=crc&0x00FF;
    crc=crc>>8;
    req[7]=crc&0x00FF;

    if(!serial.write(req,8))
    {
        qDebug()<<"error while writing port";
        return;
    }

 }
void serialmodbus::writeRequestEnergy(int slave_id)
{

    char req[]={(char)(0x01),(char)(0x04),(char)(0x00),(char)(0x1A),(char)(0x00),(char)(0x02),(char)(0x86),(char)(0x06)};//(slave_id,Functions,force_multiple_coils,No.of points Hi,No.of points Lo,Error Check (LRC or CRC))
//0F=Return Slave No Response Count

    unsigned int crc=0x00;

    req[0]=slave_id;

    crc=CRC16(req,6);
    req[6]=0x00;
    req[6]=crc&0x00FF;
    crc=crc>>8;
    req[7]=crc&0x00FF;

    if(!serial.write(req,8))
    {
        qDebug()<<"error while writing port";
        return;
    }

 }
int serialmodbus::readResponseVoltage(int time, float *unit, int id)
{
     QByteArray data;

    size=0;


    while(serial.waitForReadyRead(time));
    {
        size=serial.bytesAvailable();
        if(size<9 && size>0)
        {
            qDebug() << "ModBus Message Timeout";
        }
        qDebug() << "Size Avail" << size;
        if(size==0)
        {
           qDebug()<<"No data is available in serial port";
            return -1;
        }

        data=serial.readAll();

        if(size==9)
        {
            mytypes_l *mytypes=new mytypes_l;
            mytypes->i[0]=data.at(3);
            mytypes->i[1]=data.at(4);
            mytypes->i[2]=data.at(5);
            mytypes->i[3]=data.at(6);
            qDebug()<<mytypes->i[0]<<"-"<<mytypes->i[1]<<"-"<<mytypes->i[2]<<"-"<<mytypes->i[3];
            *unit=mytypes->f*0.01;
            int temp, i = 1, j, r;
            char hex[50];
            temp = mytypes->i[2];

              while (temp != 0)
              {
                  r = temp % 16;
                  if (r < 10)
                      hex[i++] = r + 48;
                  else
                      hex[i++] = r + 55;
                  temp = temp / 16;
              }
              for (j = i; j > 0; j--)
              {
              qDebug() << hex[j];
              }
              int temp1, i1 = 1, j1, r1;
              char hex1[50];
              temp1 = mytypes->i[3];

               while (temp1 != 0)
                {
                    r1 = temp1 % 16;
                    if (r1 < 10)
                        hex1[i1++] = r1 + 48;
                    else
                        hex1[i1++] = r1 + 55;
                    temp1 = temp1 / 16;
                }
                for (j1 = i1; j1 > 0; j1--)
                        {
                    qDebug() << hex1[j1];
                }
                char hh[4];
                hh[0]=hex[2];hh[1]=hex[1];hh[2]=hex1[2];hh[3]=hex1[1];
                qDebug()<<hh;
                int i2, r2, len2, hex2 = 0;
                len2 = 4;
                    for (i2 = 0; hh[i2] != '\0'; i2++)
                    {
                        len2--;
                        if(hh[i2] >= '0' && hh[i2] <= '9')
                            r2 = hh[i2] - 48;
                        else if(hh[i2] >= 'a' && hh[i2] <= 'f')
                                r2= hh[i2] - 87;
                             else if(hh[i2] >= 'A' && hh[i2] <= 'F')
                                    r2 = hh[i2] - 55;
                        hex2 += (r2 * pow(16,len2));
                    }
                    Voltage=hex2*0.01;
                    voltagevalue= QString::number(Voltage);
                    qDebug()<<voltagevalue << "V";
                    ui->lcdNumber_voltage->display(voltagevalue);
        }

        id|=(data.at(0)&0x00ff);

        qDebug()<<"Data From slave id..:"<<id;

        return 1;
     }

     *unit=0;
     qDebug()<<"No data is available in serial port";
     return -1;

}
int serialmodbus::readResponseCurrent(int time, float *unit, int id)
{
     QByteArray data;

    size=0;


    while(serial.waitForReadyRead(time));
    {
        size=serial.bytesAvailable();
        if(size<9 && size>0)
        {
            qDebug() << "ModBus Message Timeout";
        }
        qDebug() << "Size Avail" << size;
        if(size==0)
        {
           qDebug()<<"No data is available in serial port";
            return -1;
        }

        data=serial.readAll();

        if(size==9)
        {
            mytypes_l *mytypes=new mytypes_l;
            mytypes->i[0]=data.at(3);
            mytypes->i[1]=data.at(4);
            mytypes->i[2]=data.at(5);
            mytypes->i[3]=data.at(6);
            qDebug()<<mytypes->i[0]<<"-"<<mytypes->i[1]<<"-"<<mytypes->i[2]<<"-"<<mytypes->i[3];
            *unit=mytypes->f*0.01;
            int temp, i = 1, j, r;
            char hex[50];
            temp = mytypes->i[2];

              while (temp != 0)
              {
                  r = temp % 16;
                  if (r < 10)
                      hex[i++] = r + 48;
                  else
                      hex[i++] = r + 55;
                  temp = temp / 16;
              }
              for (j = i; j > 0; j--)
              {
              qDebug() << hex[j];
              }
              int temp1, i1 = 1, j1, r1;
              char hex1[50];
              temp1 = mytypes->i[3];

               while (temp1 != 0)
                {
                    r1 = temp1 % 16;
                    if (r1 < 10)
                        hex1[i1++] = r1 + 48;
                    else
                        hex1[i1++] = r1 + 55;
                    temp1 = temp1 / 16;
                }
                for (j1 = i1; j1 > 0; j1--)
                        {
                    qDebug() << hex1[j1];
                }
                char hh[4];
                hh[0]=hex[2];hh[1]=hex[1];hh[2]=hex1[2];hh[3]=hex1[1];
                qDebug()<<hh;
                int i2, r2, len2, hex2 = 0;
                len2 = 4;
                    for (i2 = 0; hh[i2] != '\0'; i2++)
                    {
                        len2--;
                        if(hh[i2] >= '0' && hh[i2] <= '9')
                            r2 = hh[i2] - 48;
                        else if(hh[i2] >= 'a' && hh[i2] <= 'f')
                                r2= hh[i2] - 87;
                             else if(hh[i2] >= 'A' && hh[i2] <= 'F')
                                    r2 = hh[i2] - 55;
                        hex2 += (r2 * pow(16,len2));
                    }
                    Current=hex2*0.01;
                    currentvalue= QString::number(Current);
                    qDebug()<<currentvalue << "V";
                    ui->lcdNumber_voltage->display(currentvalue);
        }

        id|=(data.at(0)&0x00ff);

        qDebug()<<"Data From slave id..:"<<id;

        return 1;
     }

     *unit=0;
     qDebug()<<"No data is available in serial port";
     return -1;

}
int serialmodbus::readResponseFreq(int time, float *unit, int id)
{
     QByteArray data;

    size=0;


    while(serial.waitForReadyRead(time));
    {
        qDebug()<<"going to read unit from meter no :"<<id;

        size=serial.bytesAvailable();
        if(size<9 && size>0)
        {
            qDebug() << "ModBus Message Timeout";
        }
        qDebug() << "Size Avail" << size;
        if(size==0)
        {
           qDebug()<<"No data is available in serial port";
            return -1;
        }

        data=serial.readAll();
        qDebug()<<"serial data"<<data;

        if(size==9)
        {
            mytypes_l *mytypes=new mytypes_l;
            mytypes->i[0]=data.at(3);
            mytypes->i[1]=data.at(4);
            mytypes->i[2]=data.at(5);
            mytypes->i[3]=data.at(6);
            qDebug()<<mytypes->i[0]<<"-"<<mytypes->i[1]<<"-"<<mytypes->i[2]<<"-"<<mytypes->i[3];
            *unit=mytypes->f*0.01;
            int temp, i = 1, j, r;
            char hex[50];
            temp = mytypes->i[2];
              while (temp != 0)
              {
                  r = temp % 16;
                  if (r < 10)
                      hex[i++] = r + 48;
                  else
                      hex[i++] = r + 55;
                  temp = temp / 16;
              }
              for (j = i; j > 0; j--)
                      {qDebug() << hex[j];}
              int temp1, i1 = 1, r1;
             char hex1[50];
              temp1 = mytypes->i[3];
                while (temp1 != 0)
                {
                    r1 = temp1 % 16;
                    if (r1 < 10)
                        hex1[i1++] = r1 + 48;
                    else
                        hex1[i1++] = r1 + 55;
                    temp1 = temp1 / 16;
                }
                char hh[4];
                hh[0]=hex[2];hh[1]=hex[1];hh[2]=hex1[2];hh[3]=hex1[1];
                qDebug()<<hh;
                int i2, r2, len2;
                int hex2 = 0;
                len2 = 4;
                    for (i2 = 0; hh[i2] != '\0'; i2++)
                    {
                        len2--;
                        if(hh[i2] >= '0' && hh[i2] <= '9')
                            r2 = hh[i2] - 48;
                        else if(hh[i2] >= 'a' && hh[i2] <= 'f')
                                r2= hh[i2] - 87;
                             else if(hh[i2] >= 'A' && hh[i2] <= 'F')
                                    r2 = hh[i2] - 55;
                        hex2 += (r2 * pow(16,len2));
                    }
                    Frequency=hex2*0.01;
                    freqvalue = QString::number(Frequency);
                     ui->lcdNumber_freq->display(freqvalue);
                    qDebug()<< freqvalue << "Hz";
        }

        id|=(data.at(0)&0x00ff);

        qDebug()<<"Data From slave id..:"<<id;

        return 1;
     }

     *unit=0;
     qDebug()<<"No data is available in serial port";
     return -1;

}

int serialmodbus::readResponseEnergy(int time, float *unit, int id)
{

    QByteArray data;

   size=0;


   while(serial.waitForReadyRead(time));
   {
       qDebug()<<"going to read unit from meter no :"<<id;

       size=serial.bytesAvailable();
       if(size<9 && size>0)
       {
           qDebug() << "ModBus Message Timeout";
       }
       qDebug() << "Size Avail" << size;
       if(size==0)
       {
          qDebug()<<"No data is available in serial port";
           return -1;
       }

       data=serial.readAll();
       qDebug()<<"serial data"<<data;

       if(size==9)
       {
           mytypes_l *mytypes=new mytypes_l;
           mytypes->i[0]=data.at(3);
           mytypes->i[1]=data.at(4);
           mytypes->i[2]=data.at(5);
           mytypes->i[3]=data.at(6);
           qDebug()<<mytypes->i[0]<<"-"<<mytypes->i[1]<<"-"<<mytypes->i[2]<<"-"<<mytypes->i[3];

           *unit=mytypes->f*0.01;

           int temp, i = 1, r;

           char hex[50];
           temp = mytypes->i[2];
             while (temp != 0)
             {
                 r = temp % 16;
                 if (r < 10)
                     hex[i++] = r + 48;
                 else
                     hex[i++] = r + 55;
                 temp = temp / 16;
             }
             int temp1, i1 = 1,r1;

             char hex1[50];
             temp1 = mytypes->i[3];
               while (temp1 != 0)
               {
                   r1 = temp1 % 16;
                   if (r1 < 10)
                       hex1[i1++] = r1 + 48;
                   else
                       hex1[i1++] = r1 + 55;
                   temp1 = temp1 / 16;
               }
               char hh[4];
               if(hex[2]>=1 && hex[2]<=9)
               {
                   hh[0]=hex[2];
               }else
               {
                   hh[0]='0';
               }

               hh[1]=hex[1];
               hh[2]=hex1[2];
               hh[3]=hex1[1];
               qDebug()<<hh;
               int i2, r2, len2, hex2 = 0;
               len2 = 4;
                   for (i2 = 0; hh[i2] != '\1'; i2++)
                   {
                       len2--;
                       if(hh[i2] >= '0' && hh[i2] <= '9')
                           r2 = hh[i2] - 48;
                       else if(hh[i2] >= 'a' && hh[i2] <= 'f')
                               r2= hh[i2] - 87;
                            else if(hh[i2] >= 'A' && hh[i2] <= 'F')
                                   r2 = hh[i2] - 55;
                       hex2 += (r2 * pow(16,len2));
                   }
                   Energy=hex2*0.01;
                   energy = QString::number(Energy);
                   ui->lcdNumber_Energy->display(energy);
                   qDebug()<< energy << "Kwh";
       }

       id|=(data.at(0)&0x00ff);

       qDebug()<<"Data From slave id..:"<<id;

       return 1;
    }

    *unit=0;
    qDebug()<<"No data is available in serial port";
    return -1;

}

unsigned int  serialmodbus::CRC16 (const  char *nData, int wLength)
{
static const unsigned int wCRCTable[] = {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

unsigned char nTemp;
unsigned int wCRCWord = 0xFFFF;

    while (wLength--)
    {
        nTemp = *nData++ ^ wCRCWord;
        wCRCWord >>= 8;
        wCRCWord  ^= wCRCTable[nTemp];
    }
    return wCRCWord;

}
void serialmodbus::start()
{
    QString Gcom;
    int Gid;
    float unit=0;
    /*........Find Genset Details ......*/
    Gcom="COM6";
    Gid=1;
    serial.close();
    serial.setPortName(Gcom);
    if(!serial.open(QIODevice::ReadWrite))
    {
        qDebug()<<"error while opening port";
    }

    else
    {
        serial.setBaudRate(QSerialPort::Baud9600);
        serial.setParity(QSerialPort::NoParity);
        serial.setDataBits(QSerialPort::Data8);
        serial.setStopBits(QSerialPort::OneStop);

        writeRequestVoltage(Gid);
        status=readResponseVoltage(1000,&unit,Gid);
        writeRequestCurrent(Gid);
        status=readResponseCurrent(1000,&unit,Gid);
        writeRequestFreq(Gid);
        status=readResponseFreq(1000,&unit,Gid);
        writeRequestEnergy(Gid);
        status=readResponseEnergy(1000,&unit,Gid);


        qDebug()<<"unit value is...:"<<unit;
    }

}
