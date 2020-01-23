#ifndef QUEUE_COMMANDS_H
#define QUEUE_COMMANDS_H

#include "globalConst.h"

// Header files //

// Global Constants //
enum Q_COMMAND { INSERT_AT_END, INSERT_BY_PRIORITY, REMOVE_FROM_FRONT, REMOVE_BY_SIZE };

// Function Prototypes //
struct PCB *queueCommands(struct PCB **processArray,int processCount, struct PCB *process,  int qCommand);
struct PCB *returnSmallestValue(struct PCB **processArray, int processCount);
void shiftProcessQueue(struct PCB **processArray, int processCount, int currIndex);

#endif // !QUEUE_COMMANDS_H
