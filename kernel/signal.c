#define __module_name__     "signal"

#include "include/signal.h"

void signal_handle(){
    if(myproc()->signal==SIGALARM){
        myproc()->alarm_flag=1;
    }
}