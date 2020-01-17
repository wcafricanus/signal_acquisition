#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "SL_Data.h"
//#include <QDebug>

#include "ext.h"
using namespace std;


void SaveRawdata(double * I ,double * Q, time_t * T, int * M, int size ,const char* DirName ,int Is_title) {
    FILE *fp;
    struct stat st = {0};
    string rstPath(app_Path);

    rstPath.append(string("/Result/"));
    //hamlet for spec path
    if (stat(rstPath.c_str(), &st) == -1) {
        mkdir(rstPath.c_str(), 0700);
    }
    rstPath.append(string("RawData"));
    if (stat(rstPath.c_str(), &st) == -1) {
        mkdir(rstPath.c_str(), 0700);
    }
    rstPath.append("/");
    rstPath.append(DirName);
    rstPath.append("_RawData.csv");

    if ((fp = fopen(rstPath.c_str(), (Is_title == 1)?"w":"a+")) != NULL) {

        for (int i = 0; i < size; i++){
            struct tm * timeinfo;
            timeinfo = localtime ( &(T[i]));
            strftime(cur_time, 70, "%Y-%m-%d %H:%M:%S", timeinfo);
            fprintf(fp, "%s,%12.8f,%12.8f,%d\n", cur_time,I[i],Q[i],M[i]);}
        fclose(fp);
    } else {
        printf("Cannot open the file : %s \n",rstPath.c_str());
    }
}
