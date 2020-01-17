#ifndef COLLECTOR_H
#define COLLECTOR_H
#include <QThread>

#include "Utility/savemanager.h"
class VitalThread;
class Collector : public QThread{
    Q_OBJECT

public:
    Collector();
    ~Collector();

signals:
    void DataSignal( double ,double ,double ,double ,double );

private slots:
    void mark();

protected:
    void run();

private:
    void get_systime();

    // ADC Parameter
    int ADC_BASE;

    // Data Variables
    double Iin;
    double Qin;
    int SampleRate;

    SaveManager* SM;


};
#endif // COLLECTOR_H
