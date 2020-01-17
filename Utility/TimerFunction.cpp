#include "TimerFunction.h"
#include "stdio.h"
#include <time.h>
#include <sys/time.h>
#include <QDebug>
TimerFunction::TimerFunction()
{


}
void TimerFunction::get_systime(char** stringBox){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    if(*stringBox != NULL){
//        *stringBox = (char*)malloc(sizeof(char)*70);
        strftime(*stringBox, 70, "%Y-%m-%d %H_%M_%S", timeinfo);
//        qDebug()<<(*stringBox);
    }/*else*/
//        qDebug()<<"char pointer not initialize with memory yet!";
    //    sprintf(this->sys_time, "%s", asctime (timeinfo));
    //    sys_time[strlen(sys_time) - 1] = 0;
}

double TimerFunction::tvsecf(){
    struct timeval tv;
    double asec;

    gettimeofday(&tv,NULL);
    asec = tv.tv_usec;
    asec /= 1e6;
    asec += tv.tv_sec;

    return asec;
}
