#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "configReader.h"
#include "programReader.h"
#include "globalConst.h"
#include "createPCB.h"
#include "runSim.h"
#include "timer.h"

#define STRING_SMALL 32
#define STRING_LARGE 128

/*
 *  Name:        main
 *  Description: Checks for the correct number of arguments, reads the first
 *               argument as a configuration file and the second argument as a
 *               program file. Prints out any errors that are reported by the
 *               file reading functions.
 */
int main(int argc, char *argv[])
{

    int simNumber;
    int configMessage;
    int numActions;
    int programMessage;
    int count;
    int processCount;


    struct Config data;
    struct Action * actions;
    struct PCB *process;
    struct PCB **processArray;
    struct timeval startData;
    char* st = malloc(100 * sizeof(char));
    char* toFile = malloc(100000 * sizeof(char));
    FILE *fp;

    // Check if there are less than or more than two arguments passed
    if (argc != 2)
    {

        // Print arguments error
        printf("\n");
        printf(" Error: Config file argument expected\n");
        printf(" Example: ./sim01 config1.cnf\n");
        printf(" Program aborted\n");
        printf("\n");
        return 0;

    }

    // A value tracking the current project simulator #
    simNumber=5;

    sprintf(st,"\n Operating System Simulator, Project %d\n", simNumber);
    sendToMonitor(st, &data);
    strcat(toFile, st);
    sprintf(st,"========================================");
    sendToMonitor(st, &data);
    strcat(toFile, st);

    // Run the readConfig function and save result
    configMessage = readConfig(argv[1], &data);

    // No problems were detected in the config file
    if (configMessage == 0)
    {

        // Allocate memory for the program actions and retrieve them
        actions = malloc(200 * sizeof(struct Action));
        processArray = malloc(50 * sizeof(struct PCB));
        numActions = 0;
        programMessage = readProgram(data.filePath, actions, &numActions);

        // No errors detected in program file
        if (programMessage == 0)
        {

            sprintf(st,"\n Program %s has been read successfully\n", data.filePath);
            sendToMonitor(st, &data);
            strcat(toFile, st);

            sprintf(st,"========================================");
            sendToMonitor(st, &data);
            strcat(toFile, st);
            sprintf(st,"\n Simulation %d Start\n", simNumber);
            sendToMonitor(st, &data);
            strcat(toFile, st);
            // "Start" timer, setting the values of the struct to the time of this simulation's start
            getTime(START, startData);


            // Run simulation from meta-data information stored in array of structs 'actions'
            for (count = 0; count < numActions; count++)
            {
                if(actions[count].commandLetter == 'S')
                {
                    if(strcmp(actions[count].operationString, "start") == 0)
                    {
                        // OS operations start here, process count initialized to '0'
                        sprintf(st, " Time: %f System Start\n",
                            getTime(LAP, startData));
                        processCount=0;
                        sendToMonitor(st, &data);
                        strcat(toFile, st);

                        sprintf( st," Time: %f, OS, Begin PCB Creation\n",
                            getTime(LAP, startData));
                        sendToMonitor(st, &data);
                        strcat(toFile, st);

                    }

                    if(strcmp(actions[count].operationString, "end") == 0)
                    {
                        // OS operations end here, stop timer 
                    }
                }

                if(actions[count].commandLetter == 'A')
                {
                    // A new process
                    if(strcmp(actions[count].operationString, "start") == 0)
                    {
                      

                        // Create a PCB initialized in 'NEW' state
                        process = createPCB(processCount, count);
                        processArray[processCount] = process;

                        // Call to calc total run time
                        // store total run time
                        process->totalTime = calcProcessTime(actions, &data, count);
                         printf("\n Process total Time : %d\n", process->totalTime);

                        sprintf(st," Time: %f, OS, process %d initialized in New state\n",
                            getTime(LAP, startData), processCount);
                        sendToMonitor(st, &data);
                        strcat(toFile, st);


                        // Change the state of the process from NEW to READY
                        changeState(processArray[processCount], &data, actions);
                        sprintf(st," Time: %f, OS, process %d set to Ready state\n",
                            getTime(LAP, startData), processCount);
                        sendToMonitor(st, &data);
                        strcat(toFile, st);
                    }

                    // End of a process reached
                    if(strcmp(actions[count].operationString, "end") == 0)
                    {
                        // Process count incremented
                        processCount++;
                    }
                }
            }

        // Send simulation info to file/monitor/both
        sprintf(st, "\n Time: %f, OS, All (%d) processes set to Ready state\n",
            getTime(LAP, startData), (processCount));
        sendToMonitor(st, &data);
        strcat(toFile, st);

	// Begin running processes with call to runSim.c's first function
        toFile = schedulingProcesses(&data, processCount, numActions, processArray, &startData, actions, toFile);

        // Send remaining simulation info to file/monitor/both
        sprintf(st,"\n Time: %f, OS, All (%d) processes complete\n",
            getTime(LAP, startData), (processCount));
        sendToMonitor(st, &data);
        strcat(toFile, st);
        sprintf(st, " Time: %f, System Stop\n",
            getTime(LAP, startData));
        getTime(STOP, startData);
        sendToMonitor(st, &data);
        strcat(toFile, st);
        sprintf(st,"========================================");
        sendToMonitor(st, &data);
        strcat(toFile, st);
        sprintf(st,"\n End Simulation %d\n", simNumber);
        if(data.logTo == TO_FILE)
        {
            printf("%s\n",st);
        }
        strcat(toFile, st);
        sprintf(st,"========================================");
        sendToMonitor(st, &data);
        strcat(toFile, st);

        if(data.logTo == TO_BOTH || data.logTo == TO_FILE)
           {
            fp = fopen (data.logPath, "w+");
            fprintf(fp ,"%s", toFile);
            fclose(fp);
           }


        }

        ///////////////////////////////
        //    Program File Errors    //
        ///////////////////////////////

        // File could not be located
        else if (programMessage == 1)
        {

            printf("\n");
            printf(" Error: File %s could not be located\n", argv[2]);
            printf(" Program loading aborted\n");
            printf("\n");

        }

        // Invalid start to file
        else if (programMessage == 2)
        {

            printf("\n");
            printf(" Error: Invalid start to program file\n");
            printf(" Program loading aborted\n");
            printf("\n");

        }

        // Invalid end to file
        else if (programMessage == 3)
        {

            printf("\n");
            printf(" Error: Invalid end to program file\n");
            printf(" Program loading aborted\n");
            printf("\n");

        }

        // Invalid action syntax
        else if (programMessage == 4)
        {

            printf("\n");
            printf("    Error: Invalid action found\n");
            printf("    Program loading aborted\n");
            printf("\n");

        }

    }

    ////////////////////////////////
    //     Config File Errors     //
    ////////////////////////////////

    // File could not be located
    else if (configMessage == 1)
    {

        printf("\n");
        printf(" Error: File %s could not be located\n", argv[1]);
        printf(" Program aborted\n");
        printf("\n");

    }

    // Invalid version number
    else if (configMessage == 2)
    {

        printf("\n");
        printf(" Error: Invalid version/phase number detected\n");
        printf(" Value should be between or equal to 0 through 10\n");
        printf(" Program aborted\n");
        printf("\n");

    }

    // Invalid CPU scheduling code
    else if (configMessage == 3)
    {

        printf("\n");
        printf(" Error: Invalid CPU scheduling code detected\n");
        printf(" Value should be one of the following:\n");
        printf("     NONE,   FCFS-N\n");
        printf("     SJF-N,  SRTF-P\n");
        printf("     FCFS-P, RR-P\n");
        printf(" Program aborted\n");
        printf("\n");

    }

    // Invalid quantum time
    else if (configMessage == 4)
    {

        printf("\n");
        printf(" Error: Invalid quantum time detected\n");
        printf(" Value should be between or equal to 0 through 100\n");
        printf(" Program aborted\n");
        printf("\n");
    }

    // Invalid Memory amount
    else if (configMessage == 5)
    {

        printf("\n");
        printf(" Error: Invalid memory amount detected");
        printf(" Value should be between or equal to 0 through 1,048,576\n");
        printf(" Program aborted\n");
        printf("\n");

    }

    // Invalid processor cycle time
    else if (configMessage == 6)
    {

        printf("\n");
        printf(" Error: Invalid processor cycle time detected\n");
        printf(" Value should be between or equal to 1 through 1,000\n");
        printf(" Program aborted\n");
        printf("\n");
    }

    // Invalid I/O cycle time
    else if (configMessage == 7)
    {

        printf("\n");
        printf("    Error: Invalid I/O cycle time detected\n");
        printf("    Value should be between or equal to 1 through 10,000");
        printf("    Program aborted\n");
        printf("\n");

    }

    // Invalid Log To
    else if (configMessage == 8)
    {

        printf("\n");
        printf(" Error: Invalid log location\n");
        printf(" Log location should be either Monitor, File, or Both\n");
        printf(" Program aborted\n");

    }

    // End program
    return 0;
}


