#include <stdio.h>
#include <stdlib.h>
#include "savemanager.h"
#include <iostream>
#include "TimerFunction.h"
#include "ext.h"
#include <unistd.h>
using namespace std;
SaveManager::SaveManager(int _Per_Save_Length , int sampleRate)
{
    this->mode = 0;
    this->max_log_length = 3600*sampleRate;
    switch(0){
    case 0:
        this->Raw_I = (double**)malloc(sizeof(double*)*2);
        this->Raw_Q = (double**)malloc(sizeof(double*)*2);
        this->Raw_T = (time_t**)malloc(sizeof(double*)*2);
        this->Mark = (int**)malloc(sizeof(double*)*2);
        for(int i = 0; i <2; i++){
            this->Raw_I[i] = (double*)malloc(sizeof(double)*_Per_Save_Length);
            this->Raw_Q[i] = (double*)malloc(sizeof(double)*_Per_Save_Length);
            this->Raw_T[i] = (time_t*)malloc(sizeof(double)*_Per_Save_Length);
            this->Mark[i] = (int*)malloc(sizeof(double)*_Per_Save_Length);
        }
        Per_Save_Length = _Per_Save_Length;
        N_process = 0;
        N_buffer = 0;
        Element_Index = 0;
        dataNum = 0;
        Is_title = 1;
        marked = false;
        break;
    case 1:
    default:

        break;
    }

    printf("Save Manager setup finished");
}
SaveManager::~SaveManager(){

    for(int i = 0; i <2; i++){
        free(this->Raw_I[i]);
        free(this->Raw_Q[i]);
        free(this->Raw_T[i]);
        free(this->Mark[i]);
    }
    free(Raw_I);
    free(Raw_Q);
    free(Raw_T);
    free(Mark);
}

void SaveManager::put(double I, double Q){
    Raw_I[N_buffer][Element_Index] = I;
    Raw_Q[N_buffer][Element_Index] = Q;
    time_t currentTime;
    Raw_T[N_buffer][Element_Index] = time(&currentTime);
    if(marked){
        Mark[N_buffer][Element_Index] = 1;
        marked = false;
    }
    else{
        Mark[N_buffer][Element_Index] = 0;
    }
    if(dataNum == 0 ){
        TimerFunction::get_systime(&sys_time);
        cout << "systime: " <<sys_time<<endl;
    }
    Element_Index++;
    dataNum++;

    if( Element_Index % Per_Save_Length == 0 ){ //Save Data to File each 30 secs
        cout <<"Save Data to File each 30 secs"<<endl;
        N_process = N_buffer;
        if(time_to_save != string(sys_time)){
            cout <<"Save to new file :"<<dataNum<<endl;
            Is_title = 1;
        }
        time_to_save = string(sys_time);
        this->SignalThread();

        // Check if reach each file limit(1 hr)
        if(dataNum % this->max_log_length == 0){
            dataNum = 0;


        }

        Element_Index = 0;
        N_buffer = !N_process;
    }

}

void SaveManager::InternalThreadEntry(){

    while (start_thread) {
        pthread_mutex_lock(&lock);
        while (isActived < 0) {
            pthread_cond_wait(&cond, &lock);
        }
        if( start_thread == false ) {
            pthread_mutex_unlock(&lock);
            break;
        }
        executed_time = tvsecf();
        /*Code Block*/
        switch(this->Is_title){

        case 0:
            SaveRawdata(Raw_I[N_process],Raw_Q[N_process], Raw_T[N_process], Mark[N_process],
                        Per_Save_Length,time_to_save.c_str(),0);
            break;
        case 1:
            SaveRawdata(Raw_I[N_process],Raw_Q[N_process], Raw_T[N_process], Mark[N_process],
                        Per_Save_Length,time_to_save.c_str(),1);
            break;
        default:
            break;
        }
        //        usleep(1000);
        Is_title = 0;
        /*Code Block*/
        isActived = -1;
        executed_time = tvsecf() - executed_time;
        cout<<"Save ["<<Per_Save_Length<<"] Data Cost "<<executed_time<<" s |[sys]: "<<sys_time<<endl;
        cout.flush();

        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);

}

void::SaveManager::mark(){
    marked = true;
}
