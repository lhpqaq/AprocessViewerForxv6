// Timer Interrupt handler


#include "include/types.h"
#include "include/param.h"
#include "include/riscv.h"
#include "include/sbi.h"
#include "include/spinlock.h"
#include "include/timer.h"
#include "include/printf.h"
#include "include/proc.h"

#define WHATIME 1

struct spinlock tickslock;
uint ticks;

void timerinit() {
    initlock(&tickslock, "time");
    #ifdef DEBUG
    printf("timerinit\n");
    #endif
}

void
set_next_timeout() {
    // There is a very strange bug,
    // if comment the `printf` line below
    // the timer will not work.

    // this bug seems to disappear automatically
    // printf("");

/*    if(ticks%10==0)
    {
        usertrap();
    }*/
    sbi_set_timer(r_time() + INTERVAL);
}

void timer_tick() {
    acquire(&tickslock);
    ticks++;
    if(myproc()){
        if(myproc()->alarm_flag){
            if(myproc()->alarm_tick==myproc()->alarm_para){
                kill(myproc()->pid);
            }
            myproc()->alarm_tick++;
        }        
    }
    wakeup(&ticks);
    release(&tickslock);
    set_next_timeout();
}

uint64 retime()
{
    uint64 now;

    if(WHATIME)
    {
        now = r_time();
    }
    else
    {
        acquire(&tickslock);
        now = ticks;
        release(&tickslock);
    }
    return now;
}

/*void test_times()
{
    struct tms t;
    long tt=times(&t);
    printf("times:%ld %ld %ld %ld \n",t.utime,t.stime,t.cutime,t.cstime);
    printf("return:%ld\n",tt);
}*/