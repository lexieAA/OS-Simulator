#ifndef RUNSIM_H
#define RUNSIM_H

#include "timer.h"

// Function prototypes 
char* schedulingProcesses(struct Config *data, int processCount, int numActions, struct PCB **processArray,
                          struct timeval *startData, struct Action *action, char* fileString);
char* runProcess(int numActions, struct Action *actions, struct Config *data, struct PCB *process,
                 struct timeval *startData, char* fileString, struct PCB **interruptQueue);
void sendToMonitor(char* display, struct Config *data);

#endif
