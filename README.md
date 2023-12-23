# Producer-Consumer Problem
A demonstration of the Producer-Consumer Problem using the pthreads API and semaphores.

----------------------------------------------------------------------------------------

**Platform Used**

 - Created in VirtualBox (Linux/Ubuntu LTS 22.04.3 (64-bit))

**How to Compile**

- Download the prog.c file
- Execute the following command in Terminal: **gcc prog.c -Wall -lthread -o prog.c**

**How to Run**

The program takes 3 command-line arguments: the number of producer threads, the number of consumer threads, and the amount of time (in seconds) that the program will run for. To execute, input the following command in Terminal with the format: **./prog.c x x x** 
- Example: ./prog.c 2 1 30 (2 producers, 1 consumer, 30 seconds)

----------------------------------------------------------------------------------------

**Credits**

Diego G. Ureno </br>
California State University, East Bay </br>
Hayward, CA </br>
