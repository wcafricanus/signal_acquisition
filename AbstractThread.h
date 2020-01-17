#ifndef ABSTRACTTHREAD_H
#define ABSTRACTTHREAD_H
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <pthread.h>
//#include <error.h>
//#include "log.h"
class AbstractThread {
public:
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    int isActived = -1;
    double executed_time = 0.0f;
    bool start_thread = false;

    AbstractThread() {
        /* empty */
    }
    virtual ~AbstractThread() {
        /* empty */
    }

    /* Returns true if the thread was successfully started, false if there was an error starting the thread */
    int StartInternalThread() {
        start_thread = true;
        int c_err = pthread_create(&_thread, NULL, InternalThreadEntryFunc, this);/* == 0*/
//        if(c_err == 0)
            c_err = pthread_detach(_thread);

        return c_err;
    }

    void SignalThread() {
        isActived = 1;
        (void) pthread_cond_signal(&cond);
    }

    /* Will not return until the internal thread has exited. */
    void WaitForInternalThreadToExit() {
        start_thread = false;
        isActived = 1;
        (void) pthread_cond_signal(&cond);
        (void) pthread_join(_thread, NULL);
    }

    double tvsecf() {
        struct timeval tv;
        double asec;

        gettimeofday(&tv, NULL);
        asec = tv.tv_usec;
        asec /= 1e6;
        asec += tv.tv_sec;

        return asec;
    }

protected:
    /* Implement this method in your subclass with the code you want your thread to run. */
    virtual void InternalThreadEntry() = 0;

private:
    static void * InternalThreadEntryFunc(void * This) {

        ((AbstractThread *)This)->InternalThreadEntry();
        return NULL;
    }

    pthread_t _thread;

};

#endif // ABSTRACTTHREAD_H
