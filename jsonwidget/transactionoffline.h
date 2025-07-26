#ifndef TRANSACTIONOFFLINE_H
#define TRANSACTIONOFFLINE_H

#include <QMainWindow>

namespace Ui {
class TransactionOffline;
}

class TransactionOffline : public QMainWindow
{
    Q_OBJECT

public:
    explicit TransactionOffline(QWidget *parent = 0);
    ~TransactionOffline();
    int ans=0;
    QString str;
    int index;

    void get_list(QString ConnectorId, QString *TransactionId);

    void on_pushButton_clicked();

    void WriteData();

    void update_list_start(QString TransactionId, QString ConnectorId, QString IdTag, QString MeterStart, QString TimeStamp);
    void update_list_metervalues(QString TimeStamp, QString Value, QString Context, QString Format, QString Measured, QString Phase, QString Location, QString Unit, QString TransactionId);
    void update_list_stop(QString TransactionId,QString ConnectorId,QString IdTag,QString TimeStamp,QString MeterStop,QString Reason,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit);
    void insert_list(QString TransactionId,QString ConnectorId,QString IdTag,QString TimeStamp,QString MeterStart,QString MeterStop,QString Reason,QString Value,QString Context,QString Format,QString Measured,QString Phase,QString Location,QString Unit,QString ReservationId,QString MeterTimeStamp,QString Status);

private:
    Ui::TransactionOffline *ui;
};

#endif // TRANSACTIONOFFLINE_H
