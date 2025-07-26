#include "clientsocket.h"
#include <QDebug>
clientsocket::clientsocket(QObject *parent) :
    QObject(parent)
{

    client = new QTcpSocket(this);
    client->abort();
    connect(client, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(client, SIGNAL(disconnected()), this, SLOT(Completed()));
}

clientsocket::~clientsocket()
{
    client->close();
}
void clientsocket::start(QString address, quint16 port, QString file)
{
    qDebug()<<address<<port<<file;
    QHostAddress addr(address);
    filename = file;
    client->connectToHost(addr, port);
    client->waitForReadyRead();
    qDebug() << "socket descriptor:"<<client->socketDescriptor();
}


void clientsocket::Completed()
{
    qDebug() << "File transfer complete";
}

void clientsocket::ReadData()
{
    QFile file(filename);
    if(!(file.open(QIODevice::Append)))
    {
        qDebug("File cannot be opened.");
        exit(0);
    }
    QByteArray read = client->read(client->bytesAvailable());
    qDebug() << "Read    : " << read.size();
    file.write(read);
    file.close();
}
