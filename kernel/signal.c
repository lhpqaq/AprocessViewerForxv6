#define __module_name__     "signal"

#include "include/signal.h"
#include "include/proc.h"
#include "include/types.h"
#include "include/trap.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/spinlock.h"
#include "include/printf.h"

void signal_handle(){
	struct proc *p = myproc();
	printf("sighandle\n");
	if(p->killed==SIGALARM&&p->alarm_tick==0){
		printf("SIGALARM\n");
		if(p->sigaction.sig_action==SIG_DFL){	
			printf("SIG_DFL\n");
			kill(p->pid, SIGTERM);
		} else if(p->sigaction.sig_action==SIG_IGN){
			printf("SIG_IGN\n");
			myproc()->killed=0;
		} else {
			printf("SIG_FUNC\n");
			myproc()->trapframe->a0=myproc()->killed;
        	myproc()->trapframe->epc=myproc()->sigaction.p;
        	myproc()->killed=0;
		}
	} else if(p->killed==SIGINT) {
		printf("SIGNALINT\n");
		if(p->sigaction.sig_action==SIG_DFL) {	
			printf("SIG_DFL\n");
			kill(p->pid, SIGTERM);
		} else if(p->sigaction.sig_action==SIG_IGN) {
			printf("SIG_IGN\n");
			myproc()->killed=0;
		} else {
			printf("SIG_FUNC\n");
			myproc()->trapframe->a0=myproc()->killed;
        	myproc()->trapframe->epc=myproc()->sigaction.p;
        	myproc()->killed=0;
		}
	}
}