#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "types.h"

// Scheduling algorithms
void run_custom_rr(Process processes[], int n, int m);
void run_mlfq(Process processes[], int n, int m);

#endif