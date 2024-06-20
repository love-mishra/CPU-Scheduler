 #include <bits/stdc++.h>

using namespace std;

int waitingTime[100], burstTime[100], arrivalTime[100], turnAroundTime[100], numberOfProcesses, processIds[100];
float averageWaitingTime[5], averageTurnAroundTime[5], idleTime[5];
vector<int> startTime, finishTime;
vector<string> processState;
void printResults() {
    cout << endl;
    cout << "Starting Time \t";
    for (int startTimePoint : startTime) {
        cout << startTimePoint << "\t";
    }
    cout << endl;
    
    cout << "Finishing Time \t";
    for (int finishTimePoint : finishTime) {
        cout << finishTimePoint << "\t";
    }
    cout << endl;
    
    cout << "State\t\t";
    for (const string& state : processState) {
        cout << state << "\t";
    }
    cout << endl << endl;
}
void inputProcessDetails() {
    int inputChoice;
    cout << "Enter 0 for offline process input, 1 for online process input: ";
    cin >> inputChoice;

    if (inputChoice == 0) {
        cout << "Enter Number of processes: ";
        cin >> numberOfProcesses;

        vector<pair<pair<int,int>,int>> processInfo(numberOfProcesses);

        for (int i = 0; i < numberOfProcesses; i++) {
            processIds[i] = i + 1;
            
            cout << "Enter Burst Time of process " << i+1 << ": ";
            cin >> burstTime[i];
            processInfo[i].first.second = burstTime[i];
            
            cout << "Enter Arrival Time of process " << i+1 << ": ";
            cin >> arrivalTime[i];
            processInfo[i].first.first = arrivalTime[i];
            
            processInfo[i].second = i + 1;
        }
    } else {
        int simulationTime = 10;
        cout << "Enter the simulation time for process arrivals: ";
        cin >> simulationTime;

        for (int currentTime = 0; currentTime <= simulationTime; currentTime++) {
            int newProcessCount;
            cout << "Enter the number of processes arriving at time " << currentTime << ": ";
            cin >> newProcessCount;
            numberOfProcesses += newProcessCount;

            for (int j = 1; j <= newProcessCount; j++) {
                int processId;
                cout << "Enter the process id for arriving process: ";
                cin >> processId;
                cout << "Enter the burst time for process id " << processId << ": ";
                cin >> burstTime[processId-1];
                arrivalTime[processId-1] = currentTime;
            }
        }
    }

    vector<pair<pair<int,int>,int>> processInfo(numberOfProcesses);
    for (int i = 0; i < numberOfProcesses; i++) {
        processInfo[i].first.first = arrivalTime[i];
        processInfo[i].first.second = burstTime[i];
        processInfo[i].second = i + 1;
    }

    sort(processInfo.begin(), processInfo.end());
    
    for (int i = 0; i < numberOfProcesses; i++) {
        processIds[i] = processInfo[i].second;
        burstTime[i] = processInfo[i].first.second;
        arrivalTime[i] = processInfo[i].first.first;
    }

    for (int i = 0; i < 5; i++) {
        averageWaitingTime[i] = 0.0;
        averageTurnAroundTime[i] = 0.0;
    }
}

void firstComeFirstServe() {
    startTime.clear();
    finishTime.clear();
    processState.clear();
    
    int currentTime = arrivalTime[0] + burstTime[0];
    idleTime[0] = arrivalTime[0];
    
    if (arrivalTime[0] > 0) {
        startTime.push_back(0);
        finishTime.push_back(arrivalTime[0]);
        processState.push_back("Idle");
    }
    
    startTime.push_back(arrivalTime[0]);
    finishTime.push_back(currentTime);
    processState.push_back("Id[" + to_string(processIds[0]) + "]");
    
    waitingTime[0] = 0;
    averageTurnAroundTime[0] = turnAroundTime[0] = burstTime[0];
    
    for (int i = 1; i < numberOfProcesses; i++) {
        if (arrivalTime[i] <= currentTime) {
            startTime.push_back(currentTime);
            waitingTime[i] = currentTime - arrivalTime[i];
            currentTime += burstTime[i];
            finishTime.push_back(currentTime);
            processState.push_back("Id[" + to_string(processIds[i]) + "]");
            averageWaitingTime[0] += waitingTime[i];
            turnAroundTime[i] = waitingTime[i] + burstTime[i];
            averageTurnAroundTime[0] += turnAroundTime[i];
        } else {
            startTime.push_back(currentTime);
            finishTime.push_back(arrivalTime[i]);
            processState.push_back("Idle");
            startTime.push_back(arrivalTime[i]);
            idleTime[0] += arrivalTime[i] - currentTime;
            currentTime = burstTime[i] + arrivalTime[i];
            finishTime.push_back(currentTime);
            processState.push_back("Id[" + to_string(processIds[i]) + "]");
            waitingTime[i] = 0;
            averageWaitingTime[0] += waitingTime[i];
            turnAroundTime[i] = waitingTime[i] + burstTime[i];
            averageTurnAroundTime[0] += turnAroundTime[i];
        }
    }
    
    averageTurnAroundTime[0] /= numberOfProcesses;
    averageWaitingTime[0] /= numberOfProcesses;
    
    printResults();
}

int findMedianBurstTime() {
    vector<int> sortedBurstTimes;
    for (int i = 0; i < numberOfProcesses; i++) {
        if (burstTime[i] > 0) {
            sortedBurstTimes.push_back(burstTime[i]);
        }
    }
    sort(sortedBurstTimes.begin(), sortedBurstTimes.end());
    int size = sortedBurstTimes.size();
    if (size % 2 == 1) {
        return sortedBurstTimes[size / 2];
    }
    return (sortedBurstTimes[size / 2] + sortedBurstTimes[size / 2 - 1]) / 2;
}

void innovativeAlgorithm() {
    startTime.clear();
    finishTime.clear();
    processState.clear();
    int completedProcesses = 0, currentTime = 0, shortestJob = 0;
    int totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<int> remainingBurstTime(numberOfProcesses);
    
    for (int i = 0; i < numberOfProcesses; i++) {
        remainingBurstTime[i] = burstTime[i];
    }
    
    printf("\nProcess ID\tArrival Time\tBurst Time\tTurnaround Time\tWaiting Time\n");
    
    while (completedProcesses != numberOfProcesses) {
        int shortestRemainingTime = INT_MAX;
        bool foundProcess = false;
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (arrivalTime[i] <= currentTime && remainingBurstTime[i] > 0) {
                foundProcess = true;
            }
        }
        
        if (!foundProcess) {
            int earliestArrival = INT_MAX;
            for (int i = 0; i < numberOfProcesses; i++) {
                if (remainingBurstTime[i] > 0) {
                    earliestArrival = min(earliestArrival, arrivalTime[i]);
                }
            }
            startTime.push_back(currentTime);
            processState.push_back("Idle");
            idleTime[4] += earliestArrival - currentTime;
            currentTime = earliestArrival;
            finishTime.push_back(currentTime);
        }
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (arrivalTime[i] <= currentTime && remainingBurstTime[i] < shortestRemainingTime && remainingBurstTime[i] > 0) {
                shortestRemainingTime = remainingBurstTime[i];
                shortestJob = i;
            }
        }
        
        int timeQuantum = findMedianBurstTime();
        
        if (remainingBurstTime[shortestJob] <= timeQuantum && remainingBurstTime[shortestJob] > 0) {
            startTime.push_back(currentTime);
            currentTime += remainingBurstTime[shortestJob];
            finishTime.push_back(currentTime);
            processState.push_back("Id[" + to_string(processIds[shortestJob]) + "]");
            remainingBurstTime[shortestJob] = 0;
            completedProcesses++;
            
            int turnaroundTime = currentTime - arrivalTime[shortestJob];
            int waitingTime = turnaroundTime - burstTime[shortestJob];
            
            printf("Process[%d]\t\t%d\t\t%d\t\t%d\t\t%d\n", 
                   processIds[shortestJob], arrivalTime[shortestJob], burstTime[shortestJob], 
                   turnaroundTime, waitingTime);
            
            totalWaitingTime += waitingTime;
            totalTurnAroundTime += turnaroundTime;
        } 
        else if (remainingBurstTime[shortestJob] > 0) {
            startTime.push_back(currentTime);
            remainingBurstTime[shortestJob] -= timeQuantum;
            currentTime += timeQuantum;
            finishTime.push_back(currentTime);
            processState.push_back("Id[" + to_string(processIds[shortestJob]) + "]");
        }
    }
    
    averageWaitingTime[4] = totalWaitingTime * 1.0 / numberOfProcesses;
    averageTurnAroundTime[4] = totalTurnAroundTime * 1.0 / numberOfProcesses;
    
    for (int i = 0; i < numberOfProcesses; i++) {
        burstTime[i] = remainingBurstTime[i];
    }
    
    printResults();
}

void roundRobin() {
    startTime.clear();
    finishTime.clear();
    processState.clear();
    int totalTime = 0, completedProcesses = 0, timeQuantum = 0;
    int totalWaitingTime = 0, totalTurnAroundTime = 0;
    vector<int> remainingBurstTime(numberOfProcesses);
    
    for (int i = 0; i < numberOfProcesses; i++) {
        remainingBurstTime[i] = burstTime[i];
    }
    
    printf("\nEnter Time Quantum:\t");
    scanf("%d", &timeQuantum);
    
    printf("\nProcess ID\tArrival Time\tCompletion Time\tBurst Time\tTurnaround Time\tWaiting Time\n");
    
    for (int currentTime = 0, i = 0; completedProcesses != numberOfProcesses;) {
        if (arrivalTime[i] <= currentTime) {
            if (remainingBurstTime[i] <= timeQuantum && remainingBurstTime[i] > 0) {
                startTime.push_back(currentTime);
                currentTime += remainingBurstTime[i];
                finishTime.push_back(currentTime);
                processState.push_back("Id[" + to_string(processIds[i]) + "]");
                remainingBurstTime[i] = 0;
                completedProcesses++;
                
                int turnaroundTime = currentTime - arrivalTime[i];
                int waitingTime = turnaroundTime - burstTime[i];
                
                printf("Process[%d]\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
                       processIds[i], arrivalTime[i], currentTime, burstTime[i], 
                       turnaroundTime, waitingTime);
                
                totalWaitingTime += waitingTime;
                totalTurnAroundTime += turnaroundTime;
            } 
            else if (remainingBurstTime[i] > 0) {
                startTime.push_back(currentTime);
                remainingBurstTime[i] -= timeQuantum;
                currentTime += timeQuantum;
                finishTime.push_back(currentTime);
                processState.push_back("Id[" + to_string(processIds[i]) + "]");
            }
        }
        else {
            startTime.push_back(currentTime);
            finishTime.push_back(arrivalTime[i]);
            processState.push_back("Idle");
            idleTime[2] += arrivalTime[i] - currentTime;
            currentTime = arrivalTime[i];
        }
        
        if (i == numberOfProcesses - 1) {
            i = 0;
        } else {
            i++;
        }
    }
    
    averageWaitingTime[2] = totalWaitingTime * 1.0 / numberOfProcesses;
    averageTurnAroundTime[2] = totalTurnAroundTime * 1.0 / numberOfProcesses;
    
    printResults();
}

void shortestRemainingTimeFirst() {
    startTime.clear();
    finishTime.clear();
    processState.clear();
    int completedProcesses = 0, currentTime = 0, shortestJob = 0;
    vector<int> remainingBurstTime(numberOfProcesses);
    double totalWaitingTime = 0, totalTurnAroundTime = 0;
    
    for (int i = 0; i < numberOfProcesses; i++) {
        remainingBurstTime[i] = burstTime[i];
    }
    
    printf("\nProcess ID\tArrival Time\tCompletion Time\tBurst Time\tTurnaround Time\tWaiting Time\n");
    
    while (completedProcesses != numberOfProcesses) {
        int shortestRemainingTime = INT_MAX;
        bool foundProcess = false;
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (arrivalTime[i] <= currentTime && remainingBurstTime[i] > 0) {
                foundProcess = true;
            }
        }
        
        if (!foundProcess) {
            int earliestArrival = INT_MAX;
            for (int i = 0; i < numberOfProcesses; i++) {
                if (remainingBurstTime[i] > 0) {
                    earliestArrival = min(earliestArrival, arrivalTime[i]);
                }
            }
            startTime.push_back(currentTime);
            processState.push_back("Idle");
            idleTime[3] += earliestArrival - currentTime;
            currentTime = earliestArrival;
            finishTime.push_back(currentTime);
        }
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (arrivalTime[i] <= currentTime && remainingBurstTime[i] < shortestRemainingTime && remainingBurstTime[i] > 0) {
                shortestRemainingTime = remainingBurstTime[i];
                shortestJob = i;
            }
        }
        
        remainingBurstTime[shortestJob]--;
        startTime.push_back(currentTime);
        
    finishTime.push_back(currentTime + 1);
    processState.push_back("Id[" + to_string(processIds[shortestJob]) + "]");
    
    if (remainingBurstTime[shortestJob] == 0) {
        completedProcesses++;
        int completionTime = currentTime + 1;
        int turnaroundTime = completionTime - arrivalTime[shortestJob];
        int waitingTime = turnaroundTime - burstTime[shortestJob];
        
        printf("Process[%d]\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               processIds[shortestJob], arrivalTime[shortestJob], completionTime, 
               burstTime[shortestJob], turnaroundTime, waitingTime);
        
        totalWaitingTime += waitingTime;
        totalTurnAroundTime += turnaroundTime;
    }
    
    currentTime++;
}

averageWaitingTime[3] = totalWaitingTime / numberOfProcesses;
averageTurnAroundTime[3] = totalTurnAroundTime / numberOfProcesses;

for (int i = 0; i < numberOfProcesses; i++) {
    burstTime[i] = remainingBurstTime[i];
}

printResults();
}

void shortestJobFirst() {
    startTime.clear();
    finishTime.clear();
    processState.clear();
    int completedProcesses = 0, currentTime = 0, shortestJob = 0;
    vector<int> remainingBurstTime(numberOfProcesses);
    double totalWaitingTime = 0, totalTurnAroundTime = 0;
    
    for (int i = 0; i < numberOfProcesses; i++) {
        remainingBurstTime[i] = burstTime[i];
    }
    
    printf("\nProcess ID\tArrival Time\tCompletion Time\tBurst Time\tTurnaround Time\tWaiting Time\n");
    
    while (completedProcesses != numberOfProcesses) {
        int shortestBurstTime = INT_MAX;
        bool foundProcess = false;
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (arrivalTime[i] <= currentTime && remainingBurstTime[i] > 0) {
                foundProcess = true;
            }
        }
        
        if (!foundProcess) {
            int earliestArrival = INT_MAX;
            for (int i = 0; i < numberOfProcesses; i++) {
                if (remainingBurstTime[i] > 0) {
                    earliestArrival = min(earliestArrival, arrivalTime[i]);
                }
            }
            startTime.push_back(currentTime);
            processState.push_back("Idle");
            idleTime[1] += earliestArrival - currentTime;
            currentTime = earliestArrival;
            finishTime.push_back(currentTime);
        }
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (arrivalTime[i] <= currentTime && remainingBurstTime[i] < shortestBurstTime && remainingBurstTime[i] > 0) {
                shortestBurstTime = remainingBurstTime[i];
                shortestJob = i;
            }
        }
        
        startTime.push_back(currentTime);
        currentTime += remainingBurstTime[shortestJob];
        finishTime.push_back(currentTime);
        processState.push_back("Id[" + to_string(processIds[shortestJob]) + "]");
        
        int turnaroundTime = currentTime - arrivalTime[shortestJob];
        int waitingTime = turnaroundTime - burstTime[shortestJob];
        
        printf("Process[%d]\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               processIds[shortestJob], arrivalTime[shortestJob], currentTime, 
               burstTime[shortestJob], turnaroundTime, waitingTime);
        
        totalWaitingTime += waitingTime;
        totalTurnAroundTime += turnaroundTime;
        
        remainingBurstTime[shortestJob] = 0;
        completedProcesses++;
    }
    
    averageWaitingTime[1] = totalWaitingTime / numberOfProcesses;
    averageTurnAroundTime[1] = totalTurnAroundTime / numberOfProcesses;
    
    printResults();
}



void displayResults(int algorithmChoice) {
    printf("Average Waiting Time: %.2f\n", averageWaitingTime[algorithmChoice - 1]);
    printf("Average Turn Around Time: %.2f\n", averageTurnAroundTime[algorithmChoice - 1]);
    printf("CPU Idleness Time: %.2f\n", idleTime[algorithmChoice - 1]);
}

int main() {
    printf("Welcome to CPU Scheduling:\n\n");
    inputProcessDetails();
    int choice, continueChoice;
    printf("Choice\tAlgorithm\n1\tFirst Come First Serve\n2\tShortest Job First\n3\tRound Robin\n4\tShortest Remaining Time First\n5\tInnovative Algorithm\n");
    do {
        printf("Enter your choice from the above table: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                firstComeFirstServe();
                break;  
            case 2:
                shortestJobFirst();
                break;
            case 3:
                roundRobin();
                break;
            case 4:
                shortestRemainingTimeFirst();
                break;
            case 5:
                innovativeAlgorithm();
                break;
            default:
                printf("Please enter a choice from 1 to 5 only\n");
                break;
        }
        displayResults(choice);
        printf("\n\nEnter 1 to continue, 0 to stop: ");
        scanf("%d", &continueChoice);
    } while (continueChoice == 1);

    return 0;
}