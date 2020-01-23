#ifndef TIMER_C
#define TIMER_C

#include "timer.h"
#include "globalConst.h"

/*
 *  Name:        getTime
 *  Description: Starts, Stops, or does a lap of a timer
 *               START returns base time of 0.00000 and fills startData struct
 *               STOP returns end time of timer, halts "LAP" process
 *               LAP returns current time
 */
double getTime(int timerCommand, struct timeval startData)
{
    static Boolean running = False;
    double fpTime = 0.0;

    // will contain the time information as of the time of a function call with timerCommand 'LAP'
    struct timeval endData;

    static int startSec = 0, endSec = 0, startUSec = 0, endUSec = 0;
    int secDiff, usecDiff;

    switch(timerCommand)
    {
        case START:
            if(running == False)
            {
                // startData initialized with call to timeofday
                gettimeofday( &startData, NULL );
                running = True;
            
                startSec = startData.tv_sec;
                startUSec = startData.tv_usec;
                fpTime = (double) startSec + (double) startUSec / 1000000;
            }

            else
            {
                // Timer currently running - START not a valid timer command
                return -1;
            }

            break;

        case STOP:
            if(running == True)
            {
                gettimeofday( &startData, NULL );
                running = False;
            
                startSec = startData.tv_sec;
                startUSec = startData.tv_usec;
                fpTime = (double) startSec + (double) startUSec / 1000000;
            }

            else
            {
                // Timer currently halted - STOP not a valid timer command
                return -1;
            }
            break;

        case LAP:
            if(running == True)
            {
                gettimeofday( &endData, NULL);

                endSec = endData.tv_sec;
                endUSec = endData.tv_usec;

                secDiff = endSec - startSec;
                usecDiff = endUSec - startUSec;
               
                // borrowed straight from example "SimpleTimer" code - 
                // prevents the occasional error where values become negative
                if( usecDiff < 0 )
                {
                    usecDiff = usecDiff + 1000000;
                    secDiff = secDiff - 1;
                }

               
                fpTime = (double) secDiff + (double) usecDiff / 1000000; 
            }

            else
            {
                // Timer Stopped - LAP not a valid timer command 
                return -1;
            }
           
            break;   
     }
     return fpTime;
}

/*
 *  Name:        wait
 *  Description: Makes an operation run for a given # of milliseconds 'mSec'
 *               Does so by finding the difference from when the function is first called 
 *               and the current time until the difference is > the given # of mSec
 */
void wait(void * threadStruct)
{
    int currTime = 0;
    int secDiff, usecDiff;
    struct threadInfo *TStruct;
    TStruct = ( struct threadInfo * )threadStruct;
    int total = TStruct->currTime;

    struct timeval base; // stores time to start count from
    struct timeval now;  // stores the current time

    gettimeofday(&base, NULL);

    while(currTime < total)
    {
        gettimeofday(&now, NULL);

        secDiff = now.tv_sec - base.tv_sec; 
        usecDiff = now.tv_usec - base.tv_usec ;

        if( usecDiff < 0 )
                  {
                   usecDiff = usecDiff + 1000000;
                   secDiff = secDiff - 1;
                  } 

        // mutiplies the resulting # of seconds & divides the # of microseconds to get a millisecond value
        currTime = (secDiff*1000) + (usecDiff / 1000);
    }
}

#endif
