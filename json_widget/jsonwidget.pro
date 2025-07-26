#-------------------------------------------------
#
# Project created by QtCreator 2019-01-03T11:03:19
#
#-------------------------------------------------

QT       += core serialport

QT       += gui widgets multimedia
QT       += websockets
QT       += network
QT       += sql
QT       += printsupport

TARGET = jsonwidget
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += main.cpp\
    database.cpp \
    mainwindow2.cpp \
    mainwindow.cpp \
    filereadwrite.cpp \
    authorizeoffline.cpp \
    transactionoffline.cpp \
    clientaccess.cpp \
    dialog.cpp \
    clientsocket.cpp \
    serialmodbus.cpp

HEADERS  += \
    database.h \
    mainwindow2.h \
    mainwindow.h \
    filereadwrite.h \
    authorizeoffline.h \
    transactionoffline.h \
    clientaccess.h \
    dialog.h \
    clientsocket.h \
    serialmodbus.h

FORMS    += \
    mainwindow2.ui \
    mainwindow.ui \
    filereadwrite.ui \
    authorizeoffline.ui \
    transactionoffline.ui \
    dialog.ui \
    clientaccess.ui \
    serialmodbus.ui

RESOURCES += \
    img.qrc

RC_FILE += jsonwidget.rc
