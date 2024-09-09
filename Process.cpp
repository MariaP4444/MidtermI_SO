#include "Process.h"




Process::Process(string  p, int at, int bt, int pr){

	this->pid = p; 
	this->arrivalTime = at;
	this->burstTime = bt;
	this->remainingTime = bt;
	this->priority = pr;
	this->startTime = -1; 
	this->finishTime = -1; 
	this->waitingTime = -1;  
	this->turnaroundTime = -1; 
	          
	          
}

// Método para imprimir la información del proceso
void Process::printInfo() {
	printf("Process ID: %s, Arrival Time: %d, Burst Time: %d, Priority: %d, Start Time: %d, Finish Time: %d\n",  
			pid, arrivalTime, remainingTime, priority, startTime, finishTime);

};

