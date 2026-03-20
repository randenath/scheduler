/*Scheduler Program.
 *
 *Author: Rohan H
 *Last Modified: 02/18/26 
 *
 * CS 441/541: CPU Scheduler
 */
#include "scheduler.h"

int main(int argc, char **argv) {

	int algorithm;
	char *algoName;
	int quantum = 0;
	const char *filePath;

	for (int i = 1; i < argc; i++){
		if (strcmp(argv[i], "-s") == 0){
			algorithm = atoi(argv[i + 1]);
			if (algorithm == 1) {
				algoName = "FCFS";
			} else if (algorithm == 2) {
				algoName = "SJF";
			} else if (algorithm == 3) {
				algoName = "Priority";
			} else { 
				algoName = "RR";
			}	

			i++;

		} else if (strcmp(argv[i], "-q") == 0){
			quantum = atoi(argv[i + 1]);
			i++;

		} else {
			filePath = argv[i];
		}	
	}

	printf("Scheduler : %d %s\n", algorithm, algoName);
	printf("Quantum   : %d\n", quantum);
	printf("Sch. File : %s\n", filePath);
	printf("-------------------------------\n");


	//File IO stuff here.
	FILE *filePtr = fopen(filePath, "r");// opening file as readable.
	char line[1024];


	if (filePtr != NULL) {

		int numProcesses = 0;
		int lineNumber = 0;

		struct process *processes = NULL;

		while(fgets(line, sizeof(line), filePtr)) {
			if (lineNumber == 0) {
				sscanf(line, "%d", &numProcesses);

				processes = malloc(sizeof(struct process) * numProcesses);
				
			} else {
				int pid, burst, priority;
				sscanf(line, "%d %d %d", &pid, &burst, &priority);

				processes[lineNumber - 1].pid = pid;
				processes[lineNumber - 1].burst = burst;
				processes[lineNumber - 1].priority = priority;
			}

			lineNumber++;
		   }

		fclose(filePtr);
		
		printStartingInfo(processes, numProcesses);	

		//Algorithms Here
		
		if (algorithm == 1) {//FCFS Logic:
			
			simulateFCFS(processes, numProcesses);

			float avgWait = calculateAvgWait(processes, numProcesses);
                        float avgTurn = calculateAvgTurn(processes, numProcesses);
		
			printResults(processes, avgWait, avgTurn, numProcesses);	

			free(processes);

		} else if (algorithm == 2) {//SJF Logic:
			
			struct process *copy = malloc(sizeof(struct process) * numProcesses);

			for (int r = 0; r < numProcesses; r++) {
				copy[r] = processes[r];
			}

			for (int i = 0; i < numProcesses - 1; i++) {
				for (int j = 0; j < numProcesses - 1; j++) {
					if (copy[j].burst > copy[j + 1].burst) {
						struct process temp = copy[j];
						copy[j] = copy[j + 1];
						copy[j + 1] = temp;	
					}
				}
			}

			simulateFCFS(copy, numProcesses);

			for (int c = 0; c < numProcesses; c++) {
				for (int d = 0; d < numProcesses; d++) {
					if (processes[d].pid == copy[c].pid) {
						processes[d].waitingTime = copy[c].waitingTime;
						processes[d].turnaroundTime = copy[c].turnaroundTime;
						processes[d].completionTime = copy[c].completionTime;
						break;
					}

				}

			}

                        float avgWait = calculateAvgWait(copy, numProcesses);
                        float avgTurn = calculateAvgTurn(copy, numProcesses);

			printResults(processes, avgWait, avgTurn, numProcesses);

                        free(copy);
			free(processes);

		} else if (algorithm == 3) { //Priority Logic:

			struct process *copy = malloc(sizeof(struct process) * numProcesses);

			for (int r = 0; r < numProcesses; r++) {
                                copy[r] = processes[r];
			}

			for (int i = 0; i < numProcesses - 1; i++) {
				for (int j = 0; j < numProcesses - 1; j++) {
					if (copy[j].priority > copy[j + 1].priority) {
						struct process temp = copy[j];
						copy[j] = copy[j + 1];
						copy[j + 1] = temp;
					}
				}
			}

			simulateFCFS(copy, numProcesses);

			for (int c = 0; c < numProcesses; c++) {
                                for (int d = 0; d < numProcesses; d++) {
                                        if (processes[d].pid == copy[c].pid) {
                                                processes[d].waitingTime = copy[c].waitingTime;
                                                processes[d].turnaroundTime = copy[c].turnaroundTime;
                                                processes[d].completionTime = copy[c].completionTime;
                                                break;
                                        }
                                }
                        }


                        float avgWait = calculateAvgWait(copy, numProcesses);
			float avgTurn = calculateAvgTurn(copy, numProcesses);

			printResults(processes, avgWait, avgTurn, numProcesses);
			
			free(copy);
                        free(processes);

		} else { //Round Robin Logic: 
			for (int i = 0; i < numProcesses; i++) {
				processes[i].remainingTime = processes[i].burst;	
			}

			int *queue = malloc(sizeof(int) * numProcesses);
			int front = 0, rear = 0, queueSize = 0;

			for (int j = 0; j < numProcesses; j++) {
				queue[rear] = j;
				rear = (rear + 1) % numProcesses;
				queueSize++;
			}

			int currentTime = 0;

			while(queueSize > 0) {
				int currentIndex = queue[front];
				front = (front + 1) % numProcesses;
				queueSize--;
				
				int runTime = quantum;
				if (processes[currentIndex].remainingTime < quantum) {
					runTime = processes[currentIndex].remainingTime;
				}

				currentTime = currentTime + runTime;
				processes[currentIndex].remainingTime = processes[currentIndex].remainingTime - runTime;

				if (processes[currentIndex].remainingTime == 0) {
					processes[currentIndex].completionTime = currentTime;
					processes[currentIndex].turnaroundTime = currentTime;
				} else {
					queue[rear] = currentIndex;
					rear = (rear + 1) % numProcesses;
					queueSize++;
				}
			}

			for (int i = 0; i < numProcesses; i++) {
				processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burst;
			}



			float avgWait = calculateAvgWait(processes, numProcesses);
			float avgTurn = calculateAvgTurn(processes, numProcesses);


			printResults(processes, avgWait, avgTurn, numProcesses);

			free(queue);
			free(processes);
		}

	} else {
		fprintf(stderr, "Unable to open file!\n");
	}
    return 0;
}





//Functions. DO NOT TOUCH!!!

void simulateFCFS(struct process processes[], int numProcesses) {
	int currentTime = 0;
	for (int i = 0; i < numProcesses; i++) {
          	processes[i].completionTime = currentTime + processes[i].burst;
               	processes[i].turnaroundTime = processes[i].completionTime;
               	processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burst;

               	currentTime = processes[i].completionTime;
	}
}


float calculateAvgWait(struct process processes[], int numProcesses) {
	int totalWait = 0;
	
	for (int k = 0; k < numProcesses; k++) {
		totalWait = totalWait + processes[k].waitingTime;
	}

	float avgWait = (float) totalWait/numProcesses;

	return avgWait;
}


float calculateAvgTurn(struct process processes[], int numProcesses) {
	int totalTurnaround = 0;

	for (int j = 0; j < numProcesses; j++) {
		totalTurnaround = totalTurnaround + processes[j].turnaroundTime;
	}

	float avgTurnaround = (float) totalTurnaround/numProcesses;

	return avgTurnaround;
}


void printStartingInfo(struct process processes[], int numProcesses){
	printf("Arrival Order:  ");
	for (int i = 0; i < numProcesses; i++) {
		if (i == numProcesses - 1){
			printf("%d\n", processes[i].pid);
		} else {
			printf("%d,  ", processes[i].pid);
		}
	}
	printf("Process Information:\n");
	for (int j = 0; j < numProcesses; j++){
		printf(" %2d   %3d   %3d\n", processes[j].pid, processes[j].burst, processes[j].priority);
	}

	printf("-------------------------------\n");

}


void printResults(struct process processes[], float avgWait, float avgTurn, int numProcesses) {
	printf("Running...\n");
	printf("##################################################\n");
	printf("#  %2s  %3s  %3s   %4s  %3s\n", "#", "CPU", "Pri", "W", "T");
	for (int i = 0; i < numProcesses; i++) {
		printf("#  %2d  %3d  %3d   %4d  %3d\n", processes[i].pid, processes[i].burst, 
				processes[i].priority, processes[i].waitingTime, 
				processes[i].turnaroundTime);
	}

	printf("##################################################\n");
	printf("# Avg. Waiting Time   :   %.2f\n", avgWait);
	printf("# Avg. Turnaround Time:   %.2f\n", avgTurn);
        printf("##################################################\n");

}
