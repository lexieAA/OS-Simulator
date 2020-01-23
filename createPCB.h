#ifndef CREATEPCB_H
#define CREATEPCB_H

#include "timer.h"

// Function Prototypes

// To create PCB
struct PCB *createPCB(int processNum, int index);

// To get/ set PCB data
void changeState(struct PCB *process, struct Config *data, struct Action *actions);
int getCurrProcessTask(struct PCB *process);
void incCurrTask (struct PCB *process);
void exitProcess(struct PCB *process);
int calcProcessTime(struct Action *actions, struct Config *data, int index);

#endif
