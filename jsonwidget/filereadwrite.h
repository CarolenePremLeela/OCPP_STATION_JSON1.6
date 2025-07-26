#ifndef FILEREADWRITE_H
#define FILEREADWRITE_H

#include <QMainWindow>

namespace Ui {
class FileReadWrite;
}

class FileReadWrite : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileReadWrite(QWidget *parent = 0);
    ~FileReadWrite();
    int ans=0;
    QString str;
    int index;

    void change_key(QString key, QString newvalue);

    void get_no_of_connectors(QString key,QString *Number_of_Connectors);

    void insert_list(QString Key, QString Value);

    void on_pushButton_clicked();

    void WriteData();
private:
    Ui::FileReadWrite *ui;
};

#endif // FILEREADWRITE_H
