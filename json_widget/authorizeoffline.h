#ifndef AuthorizeOffline_H
#define AuthorizeOffline_H

#include <QMainWindow>

namespace Ui {
class AuthorizeOffline;
}

class AuthorizeOffline : public QMainWindow
{
    Q_OBJECT

public:
    explicit AuthorizeOffline(QWidget *parent = 0);
    ~AuthorizeOffline();
    int ans=0;int getlocalistversion;
    QString str;
    int index;

    void get_list(QString IdTag, QString *ExpiryDate);

    void on_pushButton_clicked();

    void WriteData();

    void Write_empty();

    void check_list(QString IdTag, int *ans);

    void update_list(QString IdTag, QString ExpiryDate, QString ParentIdTag);

    void insert_list(QString IdTag, QString ExpiryDate, QString ParentIdTag);

    void insert_diaglist(QString TimeStamp, QString Status);

    void loaddata();

    void update_list_diag(QString TimeStamp,QString Status);

    void WriteDatadiag();
private:
    Ui::AuthorizeOffline *ui;
};

#endif // AuthorizeOffline_H
