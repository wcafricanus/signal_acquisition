#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <unistd.h>
#include "AbstractThread.h"
#include "SL_Data.h"
#include <string>

class SaveManager : public AbstractThread
{
    //    Q_OBJECT
public:
    SaveManager(int _Per_Save_Length , int sampleRate);
    ~SaveManager();
    void put(double I, double Q);

protected:
    void InternalThreadEntry();
    void run();


private:
    double **Raw_I ,**Raw_Q;
    int N_process,N_buffer,dataNum,Element_Index,
    Is_title,Per_Save_Length,mode;
    int max_log_length;
    std::string time_to_save;
};

#endif // SAVEMANAGER_H
