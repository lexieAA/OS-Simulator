#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "timer.h"

char * interrupts(struct PCB **interruptQueue, int interruptCommand, struct Config *data,
	struct PCB *process, int processID, int cyclesRan, char* fileString, struct timeval *startData, struct Action *actions);

struct PCB **storeInterrupt(struct PCB **interruptQueue, struct PCB *process);

char * processInterrupts(struct PCB **interruptQueue, struct Config *data, struct timeval *startData,
	struct PCB *process, char* fileString, struct Action *actions);

void updateCycleTime( struct PCB *process, struct Config *data, struct Action *actions);

#endif 
