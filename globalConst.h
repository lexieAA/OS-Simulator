// Make sure not to compile structures more than once
#ifndef GLOBALCONST_H
#define GLOBALCONST_H

// Global Constants

enum SCH_CODE {FCFS_N, SJF_N, SRTF_P, FCFS_P, RR_P};
enum LOG_TO { TO_MONITOR, TO_FILE, TO_BOTH };
enum PROCESS_STATE { NEW, READY, RUNNING, BLOCKED, EXIT };
enum PROCESS_ARRAY_STATE {EMPTY};
enum PROCESS_STATUS {COMPLETE, IN_PROCESS, INCOMPLETE};
enum OP_CODE { BEGIN, IN, OUT, MEM, PROC, END };                     
enum INTERRUPT_COMMAND { CHECK_QUEUE, REPORT_INTERRUPT };

// Structure used to hold the data of an individual action from a program
struct Action
{

    char commandLetter;
    char operationString[16];
    int  cycleTime;

};

// Structure used to hold all necessary data read in from a configuration file
struct Config
{

    int      version;
    char     filePath[64];
    int      schCode;
    int      quanTime;
    long int memAvail;
    int      pTime;
    int      ioTime;
    int      logTo;
    char     logPath[64];

};

// Structure used to hold Process Control Block information

struct PCB
{
    int      state;
    int      processID;
    int      processIndex;
    int      totalTime; // total process cycle time 
    int      status;
};

struct threadInfo
{
    int      currTime;
};

#endif
