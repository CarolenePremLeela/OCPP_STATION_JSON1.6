#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QProcess>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->lcdNumber_rfid->display("------------------------");
    arduino = new QSerialPort(this);
    bool arduino_is_available = false;
    QString arduino_uno_port_name;
    //
    //  For each available serial port
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        //  check if the serialport has both a product identifier and a vendor identifier
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
            //  check if the product ID and the vendor ID match those of the arduino uno
            if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                    && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                arduino_is_available = true; //    arduino uno is available on this port
                arduino_uno_port_name = serialPortInfo.portName();
            }
        }
    }

    if(arduino_is_available){
        qDebug() << "Found the arduino port...\n";
        arduino->setPortName(arduino_uno_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the arduino.\n";
//        QMessageBox::information(this, "Serial Port Error", "Couldn't open serial port to arduino.");
    }
}

Dialog::~Dialog()
{
    if(arduino->isOpen()){
        arduino->close(); //    Close the serial port if it's open.
    }
    delete ui;
}

void Dialog::readSerial()
{
//    QDateTime issueDate = QDateTime::currentDateTime();
//    QString CurrentDate=issueDate.toString("yyyy-MM-dd HH:mm:ss"); // Convert currenttime into UTC time
//    QString gmt=issueDate.toString("yyyy-MM-dd HH:mm:ss");
    QStringList buffer_split = serialBuffer.split(","); //  split the serialBuffer string, parsing with ',' as the separator

    //  Check to see if there less than 3 tokens in buffer_split.
    //  If there are at least 3 then this means there were 2 commas,
    //  means there is a parsed temperature value as the second token (between 2 commas)
    if(buffer_split.length() < 5){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
        serialData = arduino->readAll();
        qDebug()<<serialData;
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{
//        // the second element of buffer_split is parsed correctly, update the temperature value on temp_lcdNumber

        qDebug() << buffer_split << "\n";
        Reader_Failure = buffer_split[4];
        ui->lcdNumber_temp->display(Reader_Failure);
        Temperature_Value = buffer_split[3];
        ui->lcdNumber_temp->display(Temperature_Value);
        Power_Switch_Failure = buffer_split[2];
        ui->lcdnumber_current->display(Power_Switch_Failure);
        Connector_Lock_Failure = buffer_split[1];
        ui->lcdnumber_voltage->display(Connector_Lock_Failure);
        Ground_Failure = buffer_split[0];
        ui->lcdnumber_powermeter->display(Ground_Failure);
        serialBuffer = "";
    }

}
