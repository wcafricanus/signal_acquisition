#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
#include <string.h>
#include "collector.h"

#define ADC
#ifdef ADC
#include "wiringPi.h"
#include "ads1115.h"
#endif
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "Utility/ext.h"
#include "TimerFunction.h"

#include <QDebug>
#define Use_Filter
using namespace std;
using namespace std::chrono;
char* sys_time;
char* cur_time;

char* workDir_path(){
    int buf_Size=100;
    char* cwd = (char*)malloc(sizeof(char)*buf_Size);
    if (getcwd(cwd, sizeof(cwd)*buf_Size) != NULL)
        return cwd;
    else
        return NULL;
}
void Collector::get_systime(){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime(sys_time, 70, "%Y-%m-%d %H:%M:%S", timeinfo);
    cout << sys_time << endl;


}
Collector::Collector() {

    this->Iin = 0;
    this->Qin = 0;
    SampleRate = 100;

#ifdef ADC
    // Setup ADC
    ADC_BASE = 2222;
    ads1115Setup (ADC_BASE, 0x48 ) ;
#endif
    //Initialize Start Time
    sys_time = (char*)malloc(sizeof(char)*70);
    cur_time = (char*)malloc(sizeof(char)*70);
    printf("Collector Thread Setup Finished.\n");
}

void Collector::run()
{
    double start , ADCTime;
    double F_I ,F_Q ,noise;

    int dataNum = 0;

    //Auto Save Thread
    //Auto Save per 30s
    SM = new SaveManager(30*SampleRate , SampleRate);
    int errSM = SM->StartInternalThread();
    if(errSM!=0){
        cout<<"Error on Create auto-save-file Thread ! : "<<errSM<<endl;
        exit(3);
    }

#ifndef ADC
    //- Initialize Seed
    srand(time(NULL));
    double rspF = 0.2,hrtF = 1.2;

#endif
    using dsec = duration<double>;
    auto t10 = milliseconds(10);
    auto t0 = steady_clock::now() + t10;
    start = TimerFunction::tvsecf();

    for(;;){

        //(Enable if using ADC)
        ADCTime = TimerFunction::tvsecf();

#ifdef ADC
        Iin = analogRead (ADC_BASE + (int)ch_I)* (ADC_PGA / 32767);
        Qin = analogRead (ADC_BASE + (int)ch_Q)* (ADC_PGA / 32767);
#else
        //Ideal Signal
        noise = (double)(rand()%1000+1)/1000000;
        Iin = 0.085+noise+0.015*sin(2*M_PI*dataNum*0.01*rspF)+0.003*sin(2*M_PI*dataNum*0.01*hrtF);
        noise = (double)(rand()%1000+1)/1000000;
        Qin = -0.065+noise+0.020*cos(2*M_PI*dataNum*0.01*rspF)+0.004*cos(2*M_PI*dataNum*0.01*hrtF);

#endif

        //Use Filter here
        F_I = Iin;
        F_Q = Qin;

        ADCTime = TimerFunction::tvsecf()-ADCTime;
        //Program Data Time counter 1 = 0.01sec ->100Hz
        dataNum++;

        //-- Call GUI & Display
        double key = (double)(dataNum)*0.01;
        if(GUI_Switch != 0)
            emit DataSignal(Iin , Qin ,F_I, F_Q , key);

        //--(1) Call Save Function
        SM->put(Iin,Qin);

        this_thread::sleep_until(t0);
        t0 += t10;
    }

}
Collector::~Collector(){
    free(sys_time);
    qDebug()<<"Destructed";
    delete(SM);
}

void Collector::mark(){
    SM->mark();
}


