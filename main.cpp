#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm> 

#include "Process.h"

using namespace std;

// Variables globales para el quantum y la lista de procesos
int quantum;
vector<Process> listProcess;  // Lista de procesos leídos del archivo
queue<Process> fcfsQueue;     // Cola para los procesos con prioridad baja (FCFS)
queue<Process> rrQueue;       // Cola para los procesos con prioridad alta (RR)


// Prototipos de funciones
bool compareByArrivalTime(const Process& a, const Process& b);
void arrColasMLQ();
void arrColasMLFQ();
void MLQ();
void MLFQ();

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

// Esta función organiza los procesos en las colas de MLQ
void arrColasMLQ(){
	// Creacion de vectores y colas 
	vector<Process> rrQueueAUX;
	vector<Process> fcfsQueueAUX;
	
	// Separar los procesos en los vectores correspondientes según su prioridad
    for (const auto& process : listProcess) {
        if (process.priority == 0) {
            rrQueueAUX.push_back(process);  // Prioridad 0, van a Round Robin
        } else if (process.priority == 1) {
            fcfsQueueAUX.push_back(process); // Prioridad 1, van a FCFS
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
		
}

// Algoritmo MLQ (Multilevel Queue)
void MLQ(){
	
	int currentTime = 0;
    int totalProcesses = listProcess.size();
    int completedProcesses = 0;
    int wtALL = 0;
    int tatALL = 0;
    float wtMID = 0.0;
    float tatMID = 0.0;
    	
	while (completedProcesses < totalProcesses) {

	    if (!rrQueue.empty()) {
	        Process currentProcess = rrQueue.front();
	        if (currentProcess.arrivalTime <= currentTime) {
	            rrQueue.pop();
	            // Registrar el tiempo de inicio si es la primera vez que ejecuta
	            if (currentProcess.startTime == -1) {
	                currentProcess.startTime = currentTime;  
	 			}
	            if (currentProcess.remainingTime > quantum) {
	                currentTime += quantum;
	                currentProcess.remainingTime -= quantum; 
	                rrQueue.push(currentProcess);  // Si no terminó, vuelve a la cola
	            }
				else {
	                currentTime += currentProcess.remainingTime;
	                currentProcess.finishTime = currentTime;  // Registrar tiempo de finalización
	                currentProcess.waitingTime = (currentTime - currentProcess.burstTime - currentProcess.arrivalTime);
	                currentProcess.turnaroundTime = (currentTime - currentProcess.arrivalTime);
	                wtALL += currentProcess.waitingTime; 
	                tatALL += currentProcess.turnaroundTime; 
	                completedProcesses++;
	                printf("Process %s | (RR)   | RT %d | CT %d | WT %d | TAT  %d | \n", 
						currentProcess.pid.c_str(), currentProcess.startTime, currentProcess.finishTime, currentProcess.waitingTime, currentProcess.turnaroundTime);
	            }
	        }
			else if (!fcfsQueue.empty() && fcfsQueue.front().arrivalTime <= currentTime) {
				// Si hay procesos en la cola de FCFS y están listos para ejecutarse
	            Process currentProcess = fcfsQueue.front();
	            fcfsQueue.pop();
	            currentProcess.startTime = currentTime;  
	            currentTime += currentProcess.remainingTime;
	            currentProcess.finishTime = currentTime; 
	            currentProcess.waitingTime = (currentTime - currentProcess.burstTime - currentProcess.arrivalTime);
	            currentProcess.turnaroundTime = (currentTime - currentProcess.arrivalTime);
	            wtALL += currentProcess.waitingTime; 
	            tatALL += currentProcess.turnaroundTime; 
	            completedProcesses++;
	            printf("Process %s | (FCFS) | RT %d | CT %d | WT %d | TAT  %d | \n", 
					currentProcess.pid.c_str(), currentProcess.startTime, currentProcess.finishTime, currentProcess.waitingTime, currentProcess.turnaroundTime);
	            
	        }
			else {
	            // Si no hay procesos listos, avanzar el tiempo hasta que llegue el siguiente proceso
	            currentTime ++;
	        }
	    }
		else if (!fcfsQueue.empty()) {
	        Process fcfsProcess = fcfsQueue.front();
	        if (fcfsProcess.arrivalTime <= currentTime) {
	            fcfsQueue.pop();
	            fcfsProcess.startTime = currentTime;  // Registrar tiempo de inicio
	            currentTime += fcfsProcess.remainingTime;
	            fcfsProcess.finishTime = currentTime;  // Registrar tiempo de finalización
	            
	            fcfsProcess.waitingTime = (currentTime - fcfsProcess.burstTime - fcfsProcess.arrivalTime);
	            fcfsProcess.turnaroundTime = (currentTime - fcfsProcess.arrivalTime);
	            wtALL += fcfsProcess.waitingTime; 
	            tatALL += fcfsProcess.turnaroundTime; 
	            completedProcesses++;
	            printf("Process %s | (FCFS) | RT %d | CT %d | WT %d | TAT  %d | \n", 
					fcfsProcess.pid.c_str(), fcfsProcess.startTime, fcfsProcess.finishTime, fcfsProcess.waitingTime, fcfsProcess.turnaroundTime);
	            
	        }
	        else {
	            // Si no hay procesos listos, avanzar el tiempo hasta que llegue el siguiente proceso
	            currentTime ++;
	        }
	    }
	    else {
	            // Si no hay procesos listos, avanzar el tiempo hasta que llegue el siguiente proceso
	            currentTime ++;
	        }
}

	    
	wtMID = (float)wtALL / totalProcesses;
	tatMID = (float)tatALL / totalProcesses;
	
	printf("The average waiting time is %.1f\n", wtMID);
	printf("The average turnaround time is %.1f\n", tatMID);
		
}

void arrColasMLFQ(){
	
	sort(listProcess.begin(), listProcess.end(), compareByArrivalTime);
	for (const auto& process : listProcess) {
	    rrQueue.push(process);
	}
}

void MLFQ(){
	
	int currentTime = 0;
    int totalProcesses = listProcess.size();
    int completedProcesses = 0;
    int wtALL = 0;
    int tatALL = 0;
    float wtMID = 0.0;
    float tatMID = 0.0;
    	
	while (completedProcesses < totalProcesses) {

	    if (!rrQueue.empty()) {
	        Process currentProcess = rrQueue.front();
	        if (currentProcess.arrivalTime <= currentTime) {
	            rrQueue.pop();
	            if (currentProcess.startTime == -1) {
	                currentProcess.startTime = currentTime;  // Registrar tiempo de inicio cuando incia por primera vez el proceso
	            }
	            if (currentProcess.remainingTime > quantum) {
	                currentTime += quantum;
	                currentProcess.remainingTime -= quantum; 
	                fcfsQueue.push(currentProcess);  // Si no terminó, vuelve a la cola
	            }
				else {
	                currentTime += currentProcess.remainingTime;
	                currentProcess.finishTime = currentTime;  // Registrar tiempo de finalización
	                currentProcess.waitingTime = (currentTime - currentProcess.burstTime - currentProcess.arrivalTime);
	                currentProcess.turnaroundTime = (currentTime - currentProcess.arrivalTime);
	                wtALL += currentProcess.waitingTime; 
	                tatALL += currentProcess.turnaroundTime; 
	                completedProcesses++;
	                printf("Process %s | (RR)   | RT %d | CT %d | WT %d | TAT  %d | \n", 
						currentProcess.pid.c_str(), currentProcess.startTime, currentProcess.finishTime, currentProcess.waitingTime, currentProcess.turnaroundTime);
	            }
	        }
			else if (!fcfsQueue.empty() && fcfsQueue.front().arrivalTime <= currentTime) {
	            Process currentProcess = fcfsQueue.front();
	            fcfsQueue.pop();
	            currentTime += currentProcess.remainingTime;
	            currentProcess.finishTime = currentTime;  // Registrar tiempo de finalización
	            currentProcess.waitingTime = (currentTime - currentProcess.burstTime - currentProcess.arrivalTime);
	            currentProcess.turnaroundTime = (currentTime - currentProcess.arrivalTime);
	            wtALL += currentProcess.waitingTime; 
	            tatALL += currentProcess.turnaroundTime; 
	            completedProcesses++;
	            printf("Process %s | (FCFS) | RT %d | CT %d | WT %d | TAT  %d | \n", 
					currentProcess.pid.c_str(), currentProcess.startTime, currentProcess.finishTime, currentProcess.waitingTime, currentProcess.turnaroundTime);
	        }
			else {
	            // Si no hay procesos listos, avanzar el tiempo hasta que llegue el siguiente proceso
	            currentTime ++;
	        }
	    }
		else if (!fcfsQueue.empty()) {
	        Process fcfsProcess = fcfsQueue.front();
	        if (fcfsProcess.arrivalTime <= currentTime) {
	            fcfsQueue.pop();
	           
	            currentTime += fcfsProcess.remainingTime;
	            fcfsProcess.finishTime = currentTime;  // Registrar tiempo de finalización
	            
	            fcfsProcess.waitingTime = (currentTime - fcfsProcess.burstTime - fcfsProcess.arrivalTime);
	            fcfsProcess.turnaroundTime = (currentTime - fcfsProcess.arrivalTime);
	            wtALL += fcfsProcess.waitingTime; 
	            tatALL += fcfsProcess.turnaroundTime; 
	            completedProcesses++;
	            printf("Process %s | (FCFS) | RT %d | CT %d | WT %d | TAT  %d | \n", 
					fcfsProcess.pid.c_str(), fcfsProcess.startTime, fcfsProcess.finishTime, fcfsProcess.waitingTime, fcfsProcess.turnaroundTime);
	            
	        }
	        else {
	            // Si no hay procesos listos, avanzar el tiempo hasta que llegue el siguiente proceso
	            currentTime ++;
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
        	
        	listProcess.clear();    
        	cout << "Ingrese el quantum: ";
        	cin >> quantum;
        	string  pid;
        	int  arrivalTime, burstTime, priority;
            while (file >> pid >> arrivalTime >> burstTime >> priority) {
	    		Process newProcess(pid, arrivalTime, burstTime, priority);
				listProcess.push_back(newProcess);
			}
			int option;
            do {
                cout << "Seleccione el algoritmo que desea ejecutar:\n";
                cout << "1. MLQ (Multilevel Queue)\n";
                cout << "2. MLFQ (Multilevel Feedback Queue)\n";
                cout << "0. Salir\n";
                cout << "Opcion: ";
                cin >> option;

                switch (option) {
                    case 1:
                        arrColasMLQ();
                        MLQ();
                        break;
                    case 2:
                        arrColasMLFQ();
                        MLFQ();
                        break;
                    case 0:
                        cout << "Saliendo...\n";
                        break;
                    default:
                        cout << "Opción no válida, por favor intente nuevamente.\n";
                        break;
                }
            } while (option != 0);

			
        }
    } while (nameFile != "0.txt");
	
	return 0;
}
