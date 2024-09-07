#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm> 

using namespace std;
int quantum;

class Process {
	public:
	    int pid;
	    int arrivalTime;
	    int burstTime;
	    int burstTimeAUX;
	    int priority;
	    int startTime;
	    int finishTime;
	    int remainingTime;
	
	    Process(int p, int at, int bt, int pr)
	        : pid(p), arrivalTime(at), burstTime(bt), priority(pr),
	          startTime(-1), finishTime(-1), remainingTime(-1), burstTimeAUX(bt) {}
	          
	    // Método para imprimir la información del proceso
	    void printInfo() const {
	        printf("Process ID: %d, Arrival Time: %d, Burst Time: %d, Priority: %d, Start Time: %d, Finish Time: %d\n",
	               pid, arrivalTime, burstTime, priority, startTime, finishTime);
	    }
};
       
queue<Process> fcfsQueue;
queue<Process> rrQueue;
vector<Process> listProcess;
    
// Comparador para ordenar por arrivalTime
bool compareByArrivalTime(const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
}


//void printQueue(std::queue<Process> cola) {
//    while (!cola.empty()) {
//        Process p = cola.front();
//        p.printInfo();
//        cola.pop();
//    }
//}

// Separa los procesos en las colas correspondientes 
void arrColas(){
	// Creacion de vectores y colas 
	vector<Process> rrQueueAUX;
	vector<Process> fcfsQueueAUX;
	// Dividir los procesos en los vectores correspondientes
    for (const auto& process : listProcess) {
        if (process.priority == 0) {
            rrQueueAUX.push_back(process);
        } else if (process.priority == 1) {
            fcfsQueueAUX.push_back(process);
        }
    }
	// Ordenar vectores por arrivalTime
    sort(rrQueueAUX.begin(), rrQueueAUX.end(), compareByArrivalTime);
    sort(fcfsQueueAUX.begin(), fcfsQueueAUX.end(), compareByArrivalTime);
	
	// Llenar las colas con los procesos desde los vectores
	for (const auto& process : fcfsQueueAUX) {
	    fcfsQueue.push(process);
	}
	for (const auto& process : rrQueueAUX) {
	    rrQueue.push(process);
	}
	
//	// Imprimir las colas
//    printf("FCFS Queue:\n");
//    printQueue(fcfsQueue);
//
//    printf("RR Queue:\n");
//    printQueue(rrQueue);

//    // Imprimir los procesos en rrQueueAUX
//    cout << "RR Queue AUX:\n";
//    for (const auto& process : rrQueueAUX) {
//        process.printInfo();
//    }
//
//    // Imprimir los procesos en fcfsQueueAUX
//    cout << "FCFS Queue AUX:\n";
//    for (const auto& process : fcfsQueueAUX) {
//        process.printInfo();
//    }
	
}




void MLQ(){
	
	int currentTime;
    int totalProcesses = listProcess.size();
    int completedProcesses = 0;
    int wtALL = 0;
    int tatALL = 0;
    
    float wtMID = 0.0;
    float tatMID = 0.0;
    
    if (rrQueue.front().arrivalTime <=  fcfsQueue.front().arrivalTime){
    	currentTime = rrQueue.front().arrivalTime;
	}
	else{
		currentTime = fcfsQueue.front().arrivalTime;
	}
	 
	
	while (completedProcesses < totalProcesses) {

    if (!rrQueue.empty()) {
        Process currentProcess = rrQueue.front();
        if (currentProcess.arrivalTime <= currentTime) {
            rrQueue.pop();
            if (currentProcess.startTime == -1) {
                currentProcess.startTime = currentTime;  // Registrar tiempo de inicio
            }
 
            if (currentProcess.burstTime > quantum) {
                currentTime += quantum;
                currentProcess.burstTime -= quantum; 
                rrQueue.push(currentProcess);  // Si no terminó, vuelve a la cola
            }
			else {
                currentTime += currentProcess.burstTime;
                currentProcess.finishTime = currentTime;  // Registrar tiempo de finalización
                wtALL += (currentTime - currentProcess.burstTimeAUX - currentProcess.arrivalTime);
                tatALL += (currentTime - currentProcess.arrivalTime);
                completedProcesses++;
                printf("Process %d (RR) start at time %d finished at time %d\n", currentProcess.pid, currentProcess.startTime, currentProcess.finishTime);
            }
        }
		else if (!fcfsQueue.empty() && fcfsQueue.front().arrivalTime <= currentTime) {
            Process currentProcess = fcfsQueue.front();
            fcfsQueue.pop();
            
            if (currentProcess.startTime == -1) {
                currentProcess.startTime = currentTime;  // Registrar tiempo de inicio
            }
            currentTime += currentProcess.burstTime;
            currentProcess.finishTime = currentTime;  // Registrar tiempo de finalización
            
            wtALL += (currentTime - currentProcess.burstTimeAUX - currentProcess.arrivalTime);
            tatALL += (currentTime - currentProcess.arrivalTime);
            completedProcesses++;
            printf("Process %d (FCFS) start at time %d finished at time %d\n", currentProcess.pid, currentProcess.startTime, currentProcess.finishTime);
        }
		else {
            // Si no hay procesos listos, avanzar el tiempo hasta que llegue el siguiente proceso
            if (!rrQueue.empty() && rrQueue.front().arrivalTime > currentTime) {
                currentTime = rrQueue.front().arrivalTime;
            } else if (!fcfsQueue.empty() && fcfsQueue.front().arrivalTime > currentTime) {
                currentTime = fcfsQueue.front().arrivalTime;
            }
        }
    }
	else if (!fcfsQueue.empty()) {
        Process fcfsProcess = fcfsQueue.front();
        if (fcfsProcess.arrivalTime <= currentTime) {
            fcfsQueue.pop();
            fcfsProcess.startTime = currentTime;  // Registrar tiempo de inicio
            currentTime += fcfsProcess.burstTime;
            fcfsProcess.finishTime = currentTime;  // Registrar tiempo de finalización
            
            wtALL += (currentTime - fcfsProcess.burstTimeAUX - fcfsProcess.arrivalTime);
            tatALL += (currentTime - fcfsProcess.arrivalTime);
            completedProcesses++;
            printf("Process %d (FCFS) start at time %d finished at time %d\n", fcfsProcess.pid, fcfsProcess.startTime, fcfsProcess.finishTime);
        }
    }
}

	    
	wtMID = (float)wtALL / totalProcesses;
	tatMID = (float)tatALL / totalProcesses;
	
	printf("The average waiting time is %.1f\n", wtMID);
	printf("The average turnaround time is %.1f\n", tatMID);
		
}


int main(){
	string nameFile; 
    do {
        cout << "Ingrese el nombre del programa (archivo) sin .txt, de lo contrario marque 0 to exit: ";
        cin >> nameFile;
        nameFile = nameFile + ".txt";
        ifstream file(nameFile.c_str());
        if (!file.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo " << nameFile << std::endl;
            return 404;  
        }
        else {
        	cout << "Ingrese el quantum: ";
        	cin >> quantum;
        	int pid, arrivalTime, burstTime, priority;
            while (file >> pid >> arrivalTime >> burstTime >> priority) {
	    		Process newProcess(pid, arrivalTime, burstTime, priority);
				listProcess.push_back(newProcess);
			}
			arrColas();
			MLQ();
        }
    } while (nameFile != "0.txt");
	
	return 0;
}
