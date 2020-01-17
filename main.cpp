#include "mainwindow.h"
#include "collector.h"
#include <QApplication>
#include <iostream>
#include <unistd.h>
using namespace std;
#include <QString>
#include <stdio.h>
#include <qdebug.h>
//Configable Variable(Default)
char app_Path[256];      //app path for log dir
int ch_I = 4 , ch_Q = 5;
double ADC_PGA = 4.096;
int GUI_Switch = 1;
int ADC_FSR = 4;


void Read_Config();

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QApplication::setGraphicsSystem("raster");
#endif

    //hamlet
    QApplication a(argc, argv);

    memset(app_Path, 0, 512);
    Read_Config();
    QString szPath = QCoreApplication::applicationDirPath();
    if (szPath.length() < 256)
        memcpy(app_Path, szPath.toStdString().c_str(), szPath.length());

    qDebug()<<szPath;
    MainWindow w;
    if(GUI_Switch != 0)
        w.show();

    //--- Create ADC Thread & initialize timer for GUI
    Collector *thread1 = new Collector();

    //--- Connect ADC Thread Signal to GUI Slot
    //- Signal&Slot : Display Raw Data
    QObject::connect(thread1,SIGNAL(DataSignal(double ,double ,double ,double ,double))
                     ,&w ,SLOT(Dataslot(double ,double ,double ,double ,double)));

    //--- Start Collecting Data from ADC
    thread1->start();

    return a.exec();
}
