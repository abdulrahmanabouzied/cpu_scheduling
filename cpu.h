#ifndef CPU_CPU_H
#define CPU_CPU_H

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <queue>
//#include <cstdlib>
#include <ctime>
#include "pcb.h"
using namespace std;


class CPU {
public:
    void takeNewProcess(int process_number, int arrival_time, int burst_time);
    void executeFCFS();
    void executeSJTNonPreemptive();
    void executeSJTPreemptive();
    void executeRR(int time_quantum);
    void displayResults();
    void showState();
    void getData();

private:
    // using vectors to ease the adding process, in the algo we used queues
    vector<PCB> ready_queue;
    vector<PCB> completion_queue;

    static double calculateAverage(int sum, int count);
};

// utilities - to show when include cpu
void setSampleData(CPU& cpu);
void testAlgorithms(CPU& cpu);

#endif //CPU_CPU_H