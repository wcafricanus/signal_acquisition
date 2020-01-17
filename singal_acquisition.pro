#-------------------------------------------------
#
# Project created by QtCreator 2018-01-20T01:35:46
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

TARGET = singal_acquisition
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
#CONFIG += static
#CONFIG += static_runtime
#QMAKE_LFLAGS += -static
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/Utility
INCLUDEPATH += $$PWD/ADC

LIBS += $$PWD/lib/libwiringPi.a

SOURCES += \
        main.cpp \
qcustomplot.cpp \
    collector.cpp \
    Utility/savemanager.cpp \
    mainwindow.cpp \
    Utility/configuration.cpp \
    Utility/TimerFunction.cpp \
    Utility/SL_Data_linux.cpp


HEADERS += \
    qcustomplot.h \
    collector.h \
    Utility/SL_Data.h \
    Utility/ext.h \
    mainwindow.h \
    Utility/savemanager.h \
    Utility/TimerFunction.h \
    AbstractThread.h \
    ADC/ads1115.h \
    ADC/wiringPi.h

FORMS += \
    mainwindow.ui

