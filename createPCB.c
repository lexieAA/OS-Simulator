#include "globalConst.h"
#include "createPCB.h"
#include <stdlib.h>
#include <stdio.h>

/*
 *  Name:        createPCB
 *  Description: Creates a process control block given a process # to use as an ID,
 *               and an index corresponding to the index of the mdf array of actions
 *               where its operations begin. Initializes the process state to 'NEW'.
 *               Returns a pointer to the PCB struct
 */
struct PCB *createPCB(int processNum, int index)
{
    struct PCB *process = malloc(sizeof(struct PCB));
    process->state = NEW;
    process->processID = processNum;
    process->processIndex = index;
    process->status = INCOMPLETE;
    return process;
} 

/*
 *  Name:        changeState
 *  Description: takes in a PCB struct and changes that PCB's process state
 *               bases change on processes current state
 */
void changeState(struct PCB *process, struct Config * data, struct Action *actions)
{

	char nextOp = actions[process->processIndex].commandLetter;

	switch (process->state)
	{
	case NEW:
		process->state = READY;
		break;
	case READY:
		process->state = RUNNING;
		break;
	case BLOCKED:
		process->state = READY;
		break;
	case RUNNING:
		// If strategy is non-preemptive, then change state to exit
		if (data->schCode == FCFS_N || data->schCode == SJF_N)
		{
			process->state = EXIT;
			exitProcess(process);
			break;
		}
		else
		{
			/*
				If the strategy is preemptive and the process sent a thread, 
				send it to blocked; it it's done doing other operations, send
				it to ready. If it's preemptive and the process is done
				running, send it to exit.
			*/
			if (nextOp == 'A')
			{
				process->state = EXIT;
				exitProcess(process);
				break;
			}
			else if (nextOp == 'I' || nextOp == 'O')
			{
				process->state = BLOCKED;
				break;
			}
			else
			{
				// Other operations
				process->state = READY;
				break;
			}
		}
	}
}

/*
 *  Name:        getCurrProcessTask
 *  Description: Given a pointer to a PCB, returns the index of the operation
 *               the process must complete next when in 'RUNNING' state
 */
int getCurrProcessTask(struct PCB *process)
{
    return process->processIndex;
}

void incCurrTask (struct PCB *process)
{
    process->processIndex +=1;
}

/*
 *  Name:        exitProcess
 *  Description: Deallocates the memory a PCB struct was given
 */
void exitProcess(struct PCB *process)
{
    free(process);
    //TODO: remove process from PCB array/ queue
}

/*
 *  Name:        calcProcessTime
 *  Description: Calculates a processes' total running time
 */
int calcProcessTime(struct Action *actions, struct Config *data, int index)
{
        int count = index+1; 
        int totalTime = 0;
     while(actions[count].commandLetter != 'A')
     {
        if(actions[count].commandLetter == 'P')
        {
            totalTime += (actions[count].cycleTime) * (data->  pTime);
        }
        if(actions[count].commandLetter == 'I'||actions[count].commandLetter == 'O')
        {
            totalTime += (actions[count].cycleTime) * (data->  ioTime);
        }
        count++;
    }
    
       return totalTime;
}

