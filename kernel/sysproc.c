#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 arg1;
  int arg2;
  uint64 arg3;

  argaddr(0, &arg1);
  argint(1, &arg2);
  argaddr(2, &arg3);

  // unsigned int abits = *(unsigned int*)arg3;
  unsigned int abits = 0;
  struct proc* cProc = myproc();
  pagetable_t pagetable = cProc->pagetable;
  // vmprint(cProc->pagetable);
  for(int i=0; i<arg2; i++, arg1+=PGSIZE){
    pte_t* tmp = walk(pagetable, arg1, 0);
    // printf("%d: pte %p pa %p\n", i, *tmp, PTE2PA(*tmp));
    if((*tmp & PTE_A) && (*tmp & PTE_D)){
      // printf("%d-- -- --: pte %p pa %p\n", i, *tmp, PTE2PA(*tmp));
      abits |= ((uint)1 << i);
    }
  }
  abits = abits - 1;
  pte_t* tmp = walk(pagetable, arg3, 0);
  uint64 pAddr = PTE2PA(*tmp) + (arg3 - PGROUNDDOWN(arg3));
  memmove((void *)pAddr, &abits, sizeof(abits));
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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
