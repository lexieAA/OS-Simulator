
#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

// Global Constants  

enum TIMER_COMMANDS { START, STOP, LAP, RESET };

typedef enum { False, True } Boolean;

// Function Prototypes 

double getTime(int timerCommand, struct timeval startData);
void wait(void * threadStruct);

#endif 
