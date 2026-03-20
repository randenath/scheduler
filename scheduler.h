/*
 * 
 *
 * CS 441/541: CPU Scheduler
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


/******************************
 * Defines
 ******************************/


/******************************
 * Structures
 ******************************/
struct process {
        int pid;
        int burst;
        int priority;
	int completionTime;
	int turnaroundTime;
	int waitingTime;
	int remainingTime;
};

/******************************
 * Global Variables
 ******************************/


/******************************
 * Function declarations
 ******************************/
void simulateFCFS(struct process processes[], int numProcesses);

float calculateAvgWait(struct process processes[], int numProcesses);

float calculateAvgTurn(struct process processes[], int numProcesses);

void printStartingInfo(struct process processes[], int numProcesses);

void printResults(struct process processes[], float avgWait, float avgTurn, int numProcesses);