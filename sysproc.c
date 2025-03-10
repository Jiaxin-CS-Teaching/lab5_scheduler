#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_shutdown(void)
{
  outw(0xB004, 0x0|0x2000);
  outw(0x604, 0x0|0x2000);

  return 0;
}

/*
 * Course: CPS3250
 *
 * Function: sys_enable_sched_trace
 * --------------------------------
 * fetch the argument from user space
 * change the value of sched_trace_enabled
 *
*/
extern int sched_trace_enabled;
int
sys_enable_sched_trace(void)
{
  if (argint(0, &sched_trace_enabled) < 0)
  {
    cprintf("enable_sched_trace() failed!\n");
  }

  return 0;
}

/*
 * Course: CPS3250
 *
 * Function: sys_set_priority
 * ----------------------------
 * fetch the arguments pid and priority from user space
 * call function set_priority
 *
*/
int sys_set_priority(void)
{
    int pid, priority;

    if(argint(0, &pid) < 0) {
        cprintf("set_priority() failed!\n");
        return -1;
    }
    if(argint(1, &priority) < 0) {
        cprintf("set_priority() failed!\n");
        return -1;
    }

    return set_priority(pid, priority);
}