#ifndef __SIGNAL_H
#define __SIGNAL_H

#include "types.h"
#include "trap.h"
#include "proc.h"
#include "signal.h"
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"


#define SIGALARM  0

void signal_handle();

#endif 