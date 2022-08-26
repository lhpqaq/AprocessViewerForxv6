#define __module_name__     "signal"

#include "include/signal.h"
#include "include/proc.h"
#include "include/types.h"
#include "include/trap.h"
#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/spinlock.h"
#include "include/printf.h"

void signal_handle(){
    if(myproc()->signal==SIGALARM){
        if(myproc()->signal_action==SIG_DFL){
            printf("SIG_DFL\n");
            myproc()->alarm_flag=1;
        } else if(myproc()->signal_action==SIG_IGN) {
            printf("SIG_IGN\n");
            myproc()->killed=0;
        } else {
            myproc()->alarm_flag=1;
        }
    }
}