
#include "include/types.h"
#include "include/riscv.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/spinlock.h"
#include "include/proc.h"
#include "include/syscall.h"
#include "include/timer.h"
#include "include/kalloc.h"
#include "include/string.h"
#include "include/printf.h"
#include "include/vm.h"
#include "include/signal.h"
#include "include/vm.h"

extern int exec(char *path, char **argv);
extern struct proc proc[NPROC];

uint64
sys_exec(void)
{
  char path[FAT32_MAX_PATH], *argv[MAXARG];
  int i;
  uint64 uargv, uarg;

  if(argstr(0, path, FAT32_MAX_PATH) < 0 || argaddr(1, &uargv) < 0){
    return -1;
  }
  memset(argv, 0, sizeof(argv));
  for(i=0;; i++){
    if(i >= NELEM(argv)){
      goto bad;
    }
    if(fetchaddr(uargv+sizeof(uint64)*i, (uint64*)&uarg) < 0){
      goto bad;
    }
    if(uarg == 0){
      argv[i] = 0;
      break;
    }
    argv[i] = kalloc();
    if(argv[i] == 0)
      goto bad;
    if(fetchstr(uarg, argv[i], PGSIZE) < 0)
      goto bad;
  }

  int ret = exec(path, argv);

  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);

  return ret;

 bad:
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);
  return -1;
}

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void){
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid, SIGTERM);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void)
{
  int mask;
  if(argint(0, &mask) < 0) {
    return -1;
  }
  myproc()->tmask = mask;
  return 0;
}


uint64
sys_getppid(void){
  return myproc()->parent->pid;
}

uint64
sys_getmem(void){
  return myproc()->sz/1024;
}

uint64 sys_times(void)
{
    uint64 tms;
    if (argaddr(0, &tms) < 0) {
        return -1;
    }

    struct proc *p = myproc();
    if (copyout2(tms, (char*)&(p->times), sizeof(p->times)) < 0) {
        return -1;
    }
    return retime();
}

uint64 
sys_alarm(void){
  int second;
  if(argint(0, &second) < 0) {
    return -1;
  }

  myproc()->signal=SIGALARM;

  myproc()->alarm_flag=1;
  myproc()->alarm_para=second*5;
  myproc()->alarm_tick=0;
  return 0;
}

uint64 
sys_pause(void){
  struct proc* p = myproc();
  printf("pause: waiting for signal to wake up!\n");
  // p->state = SLEEPING;
  // scheduler();
  acquire(&tickslock);
  while(p->killed == 0){
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  printf("111\n");
  return 0;
}

uint64 
sys_signal(void){
  int sigType;
  uint64 sigaction;
  if(argint(0, &sigType) < 0) {
    return -1;
  }
  if(argaddr(1, &sigaction) < 0){
    return -1;
  }
  printf("%d, %d\n", sigType, sigaction);
  myproc()->sigaction.sig_flags=1;
  myproc()->sigaction.sig_action=(func)sigaction;
  if((func)sigaction==SIG_FUNC){
    myproc()->sigaction.p=sigaction;
  }
  myproc()->sigaction.sig_type=sigType;
  return 0;
}


uint64 sys_procps(void)
{
  uint64 addr;
  if(argaddr(0, &addr) < 0)
    return -1;
  int pids[NPROC];
  struct procinfo pinfo[NPROC];
  int i;
  uint64 len = 0;
  int cnt = getPids(pids);
  for(i = 0; i < cnt; i++)
  {
    proc_ps(pids[i],&pinfo[i]);
    len += sizeof(pinfo[i]);
  }
  copyout2(addr,(char*)pinfo,len);
  return cnt;
}