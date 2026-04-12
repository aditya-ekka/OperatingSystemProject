#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
  int inuse[NPROC];            // whether this slot of the process table is in use (1 or 0)
  int pid[NPROC];              // PID of each process
  int state[NPROC];            // current state of each process (mapping from enum procstate)
  uint64 size[NPROC];          // size of each process's memory in bytes
  char name[NPROC][16];        // name of each process
  int priority[NPROC];         // priority of each process
};

#endif // _PSTAT_H_
