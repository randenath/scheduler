# scheduler# CS441/541 Scheduling Project

## Author(s):
Mr. Rohan H


## Date:
02/18/2026
5:39 p.m.


## Description:
This is the scheduler program. The objective of this program was to accurately program 4 different scheduling algorithms: First-Come, First-Served (FCFS), Shortest-Job First(SJF), Priority, Round-Robin(RR).

The program takes an input file and command line inputs to indicate the algorithm to use as well as optional input for the quantum value in Round-Robin.

General Program Run:
1. Parses command-line arguments to determine scheduling algorithm and quantum(optional).
2. Reads process data from the specified file.
3. Simulates the chosen scheduling algorithm.
4. Calculates completion time, waiting time, and turnaround time for each process.
5. Outputs input information as well as average waiting time and average turnaround time.

The simulation preserves input order for final output while using a sorted order for calucation when appropriate. (SJF and Priority)
## How to build the software

The included Makefile is used to build (compile and link) this software. It can be used as follows:
 - To build the software, run "make" from the command line.
 - To build the software and run the given tests, run "make check" from the command line.
 - To clean the directory, run "make clean" from the command line.


## How to use the software
The program is run through the command line. Ensure you are currently in the appropriate directory with the source files and that you used the command make. Then simply follow the following command line entry format:

Command Line Entry Format: "./scheduler -s <algorithm> [-q <quantum>] <input_file>"


## How the software was tested
The first test that I made was to test the basic functionality of the program:
3
1 5 1
2 3 2
3 8 3

The second test looks at the SJF algorithm, I look to ensure that process 2 runs first:
4
1 10 2
2 1 5
3 4 1  
4 6 3

The third test looks at tie breaking. It can be ussed in Priority and SJF:
5
1 4 2
2 4 1
3 2 3
4 2 2
5 8 1

The fourth test looked at what would happen if we gave the program minimal input. It is an interesting case that I thought would be fun to see (it was ok):
2
1 7 3
2 5 2

The fifth and final test was to be a stress test with a larger number of processes. To help with the ease of reading this readme, I have elected not to include every single process in the README:
200
1 12 3
2 8 5
3 15 1
4 10 4
5 6 2
6 9 3
7 22 5
8 14 2
.
.
.


## Test Suite
The following test files were created to verify functionality:

- **test1.txt**: 3 processes - Basic functionality test
- **test2.txt**: 4 processes - Tests SJF ordering
- **test3.txt**: 5 processes - Tests tie-breaking for equal bursts/priorities  
- **test4.txt**: 2 processes - Minimal case verification
- **test5.txt**: 200 processes - Stress test for large input handling

All test files are located in the /tests directory and can be run with any algorithm flag.

## Known bugs and problem areas
No current known bugs or problem areas. If any found, please contact the developer of the program (me) and indicate the bug. The developer shoudld get back to you shortly.
