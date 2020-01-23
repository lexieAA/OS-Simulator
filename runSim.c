#include "globalConst.h"
#include "runSim.h"
#include "interrupts.h"
#include "createPCB.h"
#include "queueCommands.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 * Name:        schedulingProcesses
 * Description: Takes in all needed information to begin running all processes,
 *              makes a call to run the processes based on the scheduling type
 */
char* schedulingProcesses(struct Config *data, int processCount, int numActions, struct PCB **processArray,
                  struct timeval *startData, struct Action *actions, char* fileString)
{
    int count;
    char* st1 = malloc(100 * sizeof(char));
    char* st2 = malloc(100 * sizeof(char));

    struct PCB *currProcess;

    struct PCB *interruptQueue;

    switch (data->schCode)
    {
    // REFERENCE: scheduling codes: FCFS_N, SJF_N, SRTF_P, FCFS_P, RR_P;
     case FCFS_N:
        for(count = 0; count < processCount; count++)
        {
            // Change the state of the process from READY to RUNNING
            changeState(processArray[count], data, actions);
            sprintf(st1, "\n Time: %f, OS, Process %d set to Running state\n\n", getTime(LAP, *startData), count);
            sendToMonitor(st1, data);

            // Call to run a process. Returns the string description of its actions 
            fileString = runProcess(numActions, actions, data, processArray[count], startData, fileString, &interruptQueue);
            
            strcat(fileString, st1);
            // Change the state of the process from RUNNING to EXIT
            sprintf( st2, " Time: %f, OS, Process %d complete, set to Exit state\n", getTime(LAP, *startData), processArray[count]->processID);
            changeState(processArray[count], data, actions);
            sendToMonitor(st2, data);
            strcat(fileString, st2);
        }
        break;
    //TODO: CPU SCHEDULING CALLS HERE
    case SJF_N:
         // TODO: CHANGE QUEUECOMMANDS TO COMPARE BY RUNTIME VALUE
         //      struct PCB *queueCommands(struct PCB **processArray, int processCount, struct PCB *process, int qCommand)
         for(count = 0; count < processCount;count++)
         { 
           currProcess = queueCommands(processArray, processCount, 0, REMOVE_BY_SIZE);
        

           // Change the state of the process from READY to RUNNING
            changeState(currProcess, data, actions);
            sprintf(st1, "\n Time: %f, OS, Process %d Chosen by SJF-N, set to Running state\n\n", getTime(LAP, *startData), currProcess->processID);
            sendToMonitor(st1, data);

            // Call to run a process. Returns the string description of its actions 
            fileString = runProcess(numActions, actions, data, currProcess, startData, fileString, &interruptQueue);

            strcat(fileString, st1);
            // Change the state of the process from RUNNING to EXIT
            sprintf( st2, " Time: %f, OS, Process %d complete, set to Exit state\n", getTime(LAP, *startData), currProcess->processID);
            changeState(currProcess, data, actions);
            
            sendToMonitor(st2, data);
            strcat(fileString, st2);

         }
         break;
    }

    return fileString;
}


/*
* Name:        runProcess
* Description: Takes in all needed information to run a single process using its PCB,
*              including the 'actions' struct containing the process operations,
*              the 'data' struct with the time information needed,
*              and the 'startData' struct containing the time as of the start of the simulation.

*/
char* runProcess(int numActions, struct Action *actions, struct Config *data, struct PCB *process,
	struct timeval *startData, char* fileString, struct PCB **interruptQueue)
{
	pthread_t thread;
	struct threadInfo currThread;

	struct threadInfo IThread;
	//struct threadInfo OThread;

	// Stores 'start' &'end' halves of an operation string description
	char* currOp = malloc(100 * sizeof(char));
	char* currOpEnd = malloc(10000 * sizeof(char));

	int processIndex = getCurrProcessTask(process) + 1;

	while (actions[processIndex].commandLetter != 'A')
	{
		if (actions[processIndex].commandLetter == 'M')
		{
			sprintf(currOp, " Time: %f, Process %d, Memory Management Allocate Action\n",
				getTime(LAP, *startData), process->processID);
			sendToMonitor(currOp, data);
			strcat(fileString, currOp);

			// Placeholder memory allocation time
			currThread.currTime = (500);
			pthread_create(&thread, NULL, (void *)&wait, (void *)&currThread);
			pthread_join(thread, NULL);
		}

		if (actions[processIndex].commandLetter == 'P')
		{
			if (data->schCode == FCFS_N || data->schCode == SJF_N)
		  {
		  sprintf(currOp, " Time: %f, Process %d, Run Operation Start\n",
			  getTime(LAP, *startData), process->processID);
		  sendToMonitor(currOp, data);
		  strcat(fileString, currOp);

		  currThread.currTime = (actions[processIndex].cycleTime) * (data->pTime);
		  pthread_create(&thread, NULL, (void *)&wait, (void *)&currThread);

		  pthread_join(thread, NULL);
		  sprintf(currOpEnd, " Time: %f, Process %d, Run Operation End\n",
			  getTime(LAP, *startData), process->processID);
		  sendToMonitor(currOp, data);
		  strcat(fileString, currOp);

		  }
			else
			{
				int cycleNum;

				// If RR-P, then cycle through until either an interrupt or quantum time is up
				if (data->schCode == RR_P)
				{
					//If the cycle time is less than the quantum time, loop according to the
					// remaining cycle time
					if (actions[processIndex].cycleTime < data->quanTime)
					{
						for (cycleNum = 1; cycleNum <= actions[processIndex].cycleTime; cycleNum++)
						{

							fileString = interrupts(interruptQueue, 0, data, process, process->processID, 
                                                                                cycleNum, fileString, startData, actions);
						}
					}
					// Otherwise, loop according to the quantum time
					else
					{
						for (cycleNum = 1; cycleNum <= data->quanTime; cycleNum++)
						{
							fileString = interrupts(interruptQueue, 0, data, process, process->processID,
                                                                                cycleNum, fileString, startData, actions);
						}

					}

					// If there were no interrupts, report that it finished running
					changeState(process, data, actions);
					sprintf(currOp, "\n Time: %f, OS, Process %d set to Ready state\n\n",
						getTime(LAP, *startData), processIndex);
					sendToMonitor(currOp, data);
					strcat(fileString, currOp);
				}
				// For the other preemptive, loop according to the cycle time
				else
				{
					for (cycleNum = 1; cycleNum <= actions[processIndex].cycleTime; cycleNum++)
					{
						fileString = interrupts(interruptQueue, 0, data, process, process->processID, 
                                                                        cycleNum, fileString, startData, actions);
					}

					// If there were no interrupts, report that it finished running
					changeState(process, data, actions);
					sprintf(currOp, "\n Time: %f, OS, Process %d set to Ready state\n\n",
						getTime(LAP, *startData), process->processID);
					sendToMonitor(currOp, data);
					strcat(fileString, currOp);
				}
			}

		}

		if (actions[processIndex].commandLetter == 'I')
		{
			if (data->schCode == SRTF_P ||
				data->schCode == FCFS_P ||
				data->schCode == RR_P)
			{
				//change process state to blocked
        			changeState(process, data, actions);
				sprintf(currOp, " Time: %f, Process %d, %s Input Start\n",
					getTime(LAP, *startData), process->processID, actions[processIndex].operationString);
				sendToMonitor(currOp, data);
				strcat(fileString, currOp);

				// create Ithread and have it report when done
				IThread.currTime = (actions[processIndex].cycleTime) * (data->ioTime);
				pthread_create(&thread, NULL, (void *)&wait, (void *)&IThread);

				// Call Interrupts function to report thread is done
				fileString = interrupts(interruptQueue, 1, data, process, process->processID, 0, fileString, startData, actions);
			}
			else
			{
				sprintf(currOp, " Time: %f, Process %d, %s Input Start\n",
					getTime(LAP, *startData), process->processID, actions[processIndex].operationString);
				sendToMonitor(currOp, data);
				strcat(fileString, currOp);

				currThread.currTime = (actions[processIndex].cycleTime) * (data->ioTime);
				pthread_create(&thread, NULL, (void *)&wait, (void *)&currThread);

				pthread_join(thread, NULL);
				sprintf(currOpEnd, " Time: %f, Process %d, %s Input End\n",
					getTime(LAP, *startData), process->processID, actions[processIndex].operationString);
				sendToMonitor(currOpEnd, data);
				strcat(fileString, currOpEnd);
			}
		}

		if (actions[processIndex].commandLetter == 'O')
		{
			if (data->schCode == SRTF_P ||
				data->schCode == FCFS_P ||
				data->schCode == RR_P)
			{
				// change process state to blocked
				changeState(process, data, actions);
				sprintf(currOp, " Time: %f, Process %d, %s Output Start\n",
					getTime(LAP, *startData), process->processID, actions[processIndex].operationString);
				sendToMonitor(currOp, data);
				strcat(fileString, currOp);

				// create Othread and have it report when done
				//OThread.currTime = (actions[processIndex].cycleTime) * (data->ioTime);
				pthread_create(&thread, NULL, (void *)&wait, (void *)&IThread);

				// Call Interrupts function to report thread is done
				fileString = interrupts(interruptQueue, 0, data, process, process->processID, 0, fileString, startData, actions);
			}
			else
			{
				sprintf(currOp, " Time: %f, Process %d, %s Output Start\n",
					getTime(LAP, *startData), process->processID, actions[processIndex].operationString);
				sendToMonitor(currOp, data);
				strcat(fileString, currOp);

				currThread.currTime = (actions[processIndex].cycleTime) * (data->ioTime);
				pthread_create(&thread, NULL, (void *)&wait, (void *)&currThread);

				pthread_join(thread, NULL);
				sprintf(currOp, " Time: %f, Process %d, %s Output End\n",
					getTime(LAP, *startData), process->processID, actions[processIndex].operationString);
				sendToMonitor(currOp, data);
				strcat(fileString, currOp);
				strcat(currOp, currOpEnd);
			}
		}

		incCurrTask(process);
		processIndex++;
	}
	return fileString;
}



/*
 *  Name:        sendToMonitor
 *  Description: Checks whether config requests data
 *               should be presented to Monitor/ Both
 *               prints a line of sim info to command line if true
 */
void sendToMonitor(char* display, struct Config *data)
{
  if(data->logTo == TO_BOTH || data->logTo == TO_MONITOR)
  {
    printf("%s", display);
  }
}
