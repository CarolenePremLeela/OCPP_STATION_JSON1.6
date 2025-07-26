#ifndef clientsocket_H
#define clientsocket_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
namespace Ui {
class clientsocket;
}

class clientsocket : public QObject
{
    Q_OBJECT

public:
    explicit clientsocket(QObject *parent = 0);
    ~clientsocket();
    QTcpSocket *client;
        void start(QString address, quint16 port, QString file);
        QString filename;
public slots:
    void ReadData();
    void Completed();
};

#endif // clientsocket_H
