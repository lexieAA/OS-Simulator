CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -c $(DEBUG)
LFLAGS = -Wall -std=c99 $(DEBUG)

sim05 : main.o configReader.o programReader.o helperFunctions.o createPCB.o timer.o runSim.o interrupts.o queueCommands.o
	$(CC) $(LFLAGS) main.o configReader.o programReader.o helperFunctions.o createPCB.o timer.o runSim.o interrupts.o queueCommands.o -lpthread -o sim05
	rm *.o

main.o : main.c configReader.h programReader.h helperFunctions.h globalConst.h createPCB.h timer.h
	$(CC) $(CFLAGS) main.c

configReader.o : configReader.c helperFunctions.h globalConst.h
	$(CC) $(CFLAGS) configReader.c

programReader.o : programReader.c helperFunctions.h globalConst.h
	$(CC) $(CFLAGS) programReader.c

helperFunctions.o : helperFunctions.c
	$(CC) $(CFLAGS) helperFunctions.c

createPCB.o : createPCB.c createPCB.h
	$(CC) $(CFLAGS) createPCB.c

SimpleTimer.o : timer.c timer.h
	$(CC) $(CFLAGS) timer.c

runSim.o : runSim.c runSim.h
	$(CC) $(CFLAGS) runSim.c

interrupts.o : interrupts.c interrupts.h
	$(CC) $(CFLAGS) interrupts.c

queueCommands.o : queueCommands.c queueCommands.h
	$(CC) $(CFLAGS) queueCommands.c

clean:
	\rm *.o sim05
