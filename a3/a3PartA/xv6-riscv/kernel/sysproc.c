#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if (argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
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
  if (argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
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

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
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
sys_howmanycmpt(void)
{
  return howmanycmpt();
}

uint64
sys_cps(void)
{
  return cps();
}

uint64
sys_waitstat(void)
{
  uint64 p,turnaround,estimate_runtime;

  if (argaddr(0, &p) < 0) {
    return -1;
  }

  if (argaddr(1, &turnaround) < 0) {
    return -1;
  }

  if (argaddr(2, &estimate_runtime) < 0) {
    return -1;
  }

  return waitstat(p, turnaround, estimate_runtime);

}

uint64
sys_btput(void)
{
  topic_t tag;
  char *message = "";

  int tag_num;
  argint(0, &tag_num);
  
  if (tag_num == 0) {
    tag = QUIZ;
  } else if (tag_num == 1) {
    tag = MIDTERM;
  } else if (tag_num == 2) {
    tag = FINAL;
  } else {
    tag = QUIZ;
  }
  
  argstr(1, message, 140);
  
  if (strlen(message) > 140) {
    printf("message is too long\n");
    exit(-1);
  }

  return btput(tag, message);
}

uint64
sys_tput(void)
{
  topic_t tag;
  char *message = "";
  
  int tag_num;
  argint(0, &tag_num);
  
  if (tag_num == 0) {
    tag = QUIZ;
  } else if (tag_num == 1) {
    tag = MIDTERM;
  } else if (tag_num == 2) {
    tag = FINAL;
  } else {
    tag = QUIZ;
  }
  
  argstr(1, message, 140);
  if (strlen(message) > 140) {
    printf("message is too long\n");
    exit(-1);
  }
  printf("Inside Kernel Message Address: %p\n", message);


  return tput(tag, message);
}

uint64
sys_btget(void)
{
  topic_t tag;
  uint64 buf;

  int tag_num;
  argint(0, &tag_num);
  
  if (tag_num == 0) {
    tag = QUIZ;
  } else if (tag_num == 1) {
    tag = MIDTERM;
  } else if (tag_num == 2) {
    tag = FINAL;
  } else {
    tag = QUIZ;
  }

  if (argaddr(1, &buf) < 0) {
    return -1;
  }
  
  return btget(tag, buf);
}

uint64
sys_tget(void)
{
  topic_t tag;
  uint64 buf;

  int tag_num;
  argint(0, &tag_num);
  
  if (tag_num == 0) {
    tag = QUIZ;
  } else if (tag_num == 1) {
    tag = MIDTERM;
  } else if (tag_num == 2) {
    tag = FINAL;
  } else {
    tag = QUIZ;
  }

  if (argaddr(1, &buf) < 0) {
    return -1;
  }

  return tget(tag, buf);
}
