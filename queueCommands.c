#include "queueCommands.h"
#include "globalConst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	Name:	     queueCommands
	Description: Take in a queue as an array of PCB structs, the length of the array, 
		     (possibly) a process to insert, and a command char array and insert the id
		     at the end or by the value; or remove the id from the front
		     or by size
                     Third argument 'process' used for insert commands only. 
*/
struct PCB *queueCommands(struct PCB **processArray, int processCount, struct PCB *process, int qCommand)
{
	int counter, count, nullIndex;
        struct PCB *pID; 
	switch (qCommand)
	{
		// If the schedule type is RR-P or SRTF-P, insert process at the end
	case INSERT_AT_END:
		counter = 0;

		// Loop through array to check for next empty spot
		while (processArray[counter] != EMPTY)
		{
			counter++;
		}
		processArray[counter]= process;
		break;

		// If the schedule type is FCFS-P, SJF-N, or FCFS-N, insert by priority
	case INSERT_BY_PRIORITY:
		// Base case: check if process ID is largest value
		count = 0;

		while ((processArray[count]->processID) > (process->processID))
		{
			count++;
		}

		// If index was NULL, insert it normally
		if (processArray[count] == EMPTY)
		{
			processArray[count]->processID = process->processID;
		}
		else
		{
			nullIndex = count;
			// Find next EMPTY index
			while (processArray[nullIndex]->processID != EMPTY)
			{
				nullIndex++;
			}

			// Go backwards until reach original counter
			while(nullIndex != count)
			{
                                nullIndex--;
				processArray[nullIndex]->processID = processArray[nullIndex]->processID;
			}

			// Set process into empty space in queue
			processArray[count]= process;

		}

		break;

		// If the schedule type is RR-P, FCFS-P, or FCFS-N, remove from the front
	case REMOVE_FROM_FRONT:

		// Pop off the first item in the array and shift 
		pID = processArray[0];

		// Test- Report that a process has been selected
		//printf("Remove from Front picked Process %d \n\n", pID->processID);

                processArray[0] = EMPTY;

		// Return the process that was removed
		return pID;

		// If the scheudle type is SRTF-P or SJF-N, remove the smallest
	case REMOVE_BY_SIZE:

		// Get the smallest value in the array.
		pID = returnSmallestValue(processArray, processCount);

		// Test-Report that a process has been selected
		//printf("Remove Smallest picked Process %d \n\n", pID->processID);
		
		return pID;
	}
 return processArray[0];//DO NOTHING WITH THIS?
}

/*
	Name:		 shiftProcessQueue
	Description: After the process is removed from the queue, it will shift
				 the process queue so that it will remove any indices with 
				 NULL in-between values.
*/
void shiftProcessQueue(struct PCB **processArray, int processCount, int currIndex)
{

       int count;

       for(count = currIndex; count < processCount-1; count++)
       {
            processArray[count] = processArray[count+1]; 
       }
        
}

/*
	Name:		 removeSmallestValue	
	Description: Test version of removeSmallestTime function for Sim. It will
				 search through the queue and return the smallest value from 
				 the queue.
*/
struct PCB *returnSmallestValue(struct PCB **processArray, int processCount)
{
	// Initialize variables
	int count = 0;
	int smallTime;
        struct PCB *smallestProcess;

        while (count < processCount )
	{
               if(processArray[count]->status == INCOMPLETE)
               {    
		 smallestProcess = processArray[count];
                 smallTime = smallestProcess->totalTime;
                 break;
              }
                count++;

	}

	// Loop while the next index in array not COMPLETE
	while (count < processCount )
	{
               if(processArray[count]->status == INCOMPLETE)
               {    
		 // If current value is greater than next value
		 if (smallTime > processArray[count]->totalTime)
		 {
			smallTime = processArray[count]->totalTime;
                        smallestProcess = processArray[count];

		 }
              }
                count++;

	}

        
          // Set the index that the process was to NULL
	  smallestProcess->status = IN_PROCESS;

        // call to shift where an extra arg = count 
       // shiftProcessQueue(processArray, processCount, count);

	return smallestProcess;
}
