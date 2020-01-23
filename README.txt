// OPERATING SYSTEMS, SPRING 2017:    SIM 05
// LAST EDITED 4/27/17
// Student course ID 771383
//
// The base of this code, including main.c, structures.h, configReader.c/.h, programReader.c/.h, makefile,
// and helperFunctions.c/.h were 'purchased' from another student with the permission of Michael Leverington
//
// As of 4/1/17 this code was created in a group of three members with the permission of Michael Leverington

 =============================================================================================================================
  SIM05 Improvements list:

- #1: "Move our interrupt-handling functions to their own file (...)" 
       We moved our set of interrupt functions from "runSim.c" to simplify testing process and facilitate the merging 
       of our group's code into working modules. 

       See two new files, "interrupts.c" and "interrupts.h", for this code. 

- #2: "Integrate the new runProcess function with calls to new interrupt.c/.h (...)"
       In the code submitted for sim04, the "runProcess" function of runSim.c would not compile without errors, so we commented 
       out the lines meant to handle the calls to interrupt processes. 
       For this part of our sim05 improvement, we rewrote these lines one at a time to resolve these unaddressed errors. 

      The edited code can be found at lines 92-275 of runSim.c, within the runProcess function

- #3: "(Edit) the structures.h file, either by removing the constants defined there to more appropriate files or by changing 
       the file name (...)"

       The file name was changed to "globalConst.h" to clarify its purpose. Constants defined in multiple files were also moved here
       and some enumerated types that were no longer used were deleted. 

- #4: "Change the 'queueCommands- RETURN_BY_SIZE' portion of queueCommands.c to compare processes by total runtime rather than by
       the placeholder value "processID" used for early testing (...)"
       These changes were needed to successfully run SJF - type scheduling by using this 'RETURN_BY_SIZE' to get the next shortest process
      
       See these changes at queueCommands.c lines 86-94, interrupts.c lines 121-132, and createPCB lines 110-128. 

- #5: "Make the project successfully run preemtive scheduling (...)" by addressing errors in logic within createPCB.c, runSim.c, and 
       interrupts.c.

       - Corrected logic within createPCB.c's 'changeState' function, which was switching to BLOCKED state under wrong conditions & 
         was incorrectly accessing the Config file's designated scheduling type.

       - Resolved logic issue within interrupts.c, where additional arguments were needed to get all the info used to interrupt a process
         (added arguments to pass config file and metadata file info to all functions...)

       - Added missing file compilation info to make file (interrupts.c/.h) 
