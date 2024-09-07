#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

class Process {
public:
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
    int startTime;
    int finishTime;
    int remainingTime;

    Process(int p, int at, int bt, int pr)
        : pid(p), arrivalTime(at), burstTime(bt), priority(pr), remainingTime(bt) {}
};

