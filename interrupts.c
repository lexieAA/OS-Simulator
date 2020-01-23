#include "globalConst.h"
#include "interrupts.h"
#include "createPCB.h"
#include "runSim.h"
#include "queueCommands.h"
#include <stdio.h>
#include <stdlib.h>

/*
Name:         interrupts
Description:  When called, it will either check if it was called already by
              a thread or processes an interrupt. It will store any interrupts
	      that were made into a queue by priority (which one interrupted
	      first) and return the queue.
*/

char * interrupts(struct PCB **interruptQueue, int interruptCommand,
	struct Config *data, struct PCB *process, int processID, int cyclesRan,
	char* fileString, struct timeval *startData, struct Action *actions)
{
	char* st1 = malloc(100 * sizeof(char));
	// If called to check for any interrupts, it will return the queue
	switch (interruptCommand)
	{
		// If called with this command, check the queue for any interrupts
		case CHECK_QUEUE:
		
			// If there any interrupts to process, change process running
			// to Ready and call processInterrupts
			if (interruptQueue[0] != EMPTY)
			{
				// Input new number of cycles left to run
				updateCycleTime(process, data, actions);
				changeState(process, data, actions);
				sprintf(st1, "\n Time: %f, OS, Process %d set to Ready state\n\n",
				getTime(LAP, *startData), processID);
				sendToMonitor(st1,data);
				strcat(fileString, st1);

				fileString = processInterrupts(interruptQueue, data, startData, process, fileString, actions);
			}

			break;
		

		// If called with this command, store the process id into the queue
		case REPORT_INTERRUPT:
		
			storeInterrupt(interruptQueue, process);
			break;

	}
	return fileString;
}


/*
  Name:		   storeInterrupt
  Description: This function will store the interrupt into the interrupt
			   array when called. It will insert the process ID that called
		       an interruption at the end of the array.
*/
struct PCB **storeInterrupt(struct PCB **interruptQueue, struct PCB *process)
{
    // Insert the process ID that reported an interruption at the end of the array
    int index = 0;

    while (interruptQueue[index] != NULL)
    {
        index++;
    }
    interruptQueue[index] = process;

    return interruptQueue;
}

/*
  Name:	        processInterrupts
  Description:  This function will loop through the Interrupt Array and
				process each interrupt by reporting that the I/O operation
				is done and change its state from Blocked to Ready.
*/
char * processInterrupts(struct PCB **interruptQueue, struct Config *data,
	struct timeval *startData, struct PCB *process, 
	char* fileString, struct Action *actions)
{
	char* st2 = malloc(100 * sizeof(char));
	int index = 0;
	// For each item in the interrupt queue
	while (interruptQueue[index] != NULL)
	{
		// Report that the I/O operation was completed
		if (actions[getCurrProcessTask(process)].commandLetter == 'I')
		{
			sprintf(st2, " Time: %f, Process %d, %s Input End\n",
			getTime(LAP, *startData), process->processID, actions[getCurrProcessTask(process)].operationString);
			sendToMonitor(st2, data);
			strcat(fileString, st2);
		}
		else
		{
			sprintf(st2, " Time: %f, Process %d, %s Output End\n",
			getTime(LAP, *startData), process->processID, actions[getCurrProcessTask(process)].operationString);
			sendToMonitor(st2, data);
			strcat(fileString, st2);
		}

		// Change its state from the Blocked to Ready and put back into process Array
		changeState(process, data, actions);

		index++;
	}
	return fileString;
}

/*
  Name:		   updateCycleTime
  Description: This function will update the cycle time of the process if it
			   was interrupted and has cycles leftover.
*/
void updateCycleTime(struct PCB *process, struct Config *data, struct Action *actions)
{

	
	/*
	// Calculate new cycle time
	int newCycleTime = getCurrProcessTask(process)->cycleTime - cycleNum;

	// Put new cycle time into the process
	getCurrProcessTask(process)->cycleTime = newCycleTime;
	*/
}
