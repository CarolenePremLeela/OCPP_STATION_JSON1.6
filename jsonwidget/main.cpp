#include "clientaccess.h"
#include <QtGlobal>
#include <QLibraryInfo>
#include <QApplication>
#include <QSplashScreen>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QPixmap pixmap(":/SplashScreen.png");
//    QSplashScreen splash(pixmap);
//    splash.show();
    clientaccess mainWin;
    //mainWin.setWindowTitle("Application");
    //sleep(5); // splash is shown for 5 seconds
//    splash.finish(&mainWin);
    clientaccess *acc=new clientaccess();
    acc->setWindowTitle("CLIENT ACCESS");
    acc->showFullScreen();
    acc->w2->w->show();
    acc->w2->show();
    return a.exec();
}
