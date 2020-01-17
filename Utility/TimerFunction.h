#ifndef TIMERFUNCTION_H
#define TIMERFUNCTION_H

#include <string>
class TimerFunction
{
public:
    TimerFunction();
    static void get_systime(char** stringBox);
    static double tvsecf();
};

#endif // TIMERFUNCTION_H
