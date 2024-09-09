#include <iostream>
#include <string>

using namespace std;

class Process {
	public:
	    string  pid;
	    int arrivalTime;
	    int remainingTime;
	    int burstTime;
	    int priority;
	    int startTime;
	    int finishTime;
	    int waitingTime;
	    int turnaroundTime;
		
		
		Process() = default; 
	    Process(string  p, int at, int bt, int pr);
	    void printInfo();

};