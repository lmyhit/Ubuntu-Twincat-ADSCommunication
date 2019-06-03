#-------------------------------------------------
#
# Project created by QtCreator 2018-01-01T08:31:48
#
#-------------------------------------------------

# Don't forget to set QMAKEFEATURES

include (C:\Users\Walden\Desktop\QtADS-master\QtADS-master\QtADS.prf)
QT       += core gui testlib serialbus serialport
CONFIG   += QtADS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtWidgetsADSExample
TEMPLATE = app


SOURCES += main.cpp\
        qadsdialog.cpp

HEADERS  += qadsdialog.h

FORMS    += qadsdialog.ui

INCLUDEPATH += C:\Users\Walden\Desktop\QtADS-master\QtADS-master
INCLUDEPATH += C:\Users\Walden\Desktop\QtADS-master\QtADS-master\src
LIBS += C:\Users\Walden\Desktop\QtADS-master\QtADS-master\lib\QtADSd.lib


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../QtADS-master/QtADS-master/lib/ -lQtADS
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../QtADS-master/QtADS-master/lib/ -lQtADSd

INCLUDEPATH += $$PWD/../QtADS-master/QtADS-master
DEPENDPATH += $$PWD/../QtADS-master/QtADS-master
