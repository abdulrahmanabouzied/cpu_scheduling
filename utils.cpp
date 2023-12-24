#include "cpu.h"

void setSampleData(CPU& cpu) {
    // Sample data for testing
    cpu.takeNewProcess(1, 0, 6);
    cpu.takeNewProcess(2, 2, 4);
    cpu.takeNewProcess(3, 4, 8);
    cpu.takeNewProcess(4, 6, 5);
    cpu.takeNewProcess(5, 8, 7);
    cpu.showState();
}

void testAlgorithms(CPU& cpu) {
    int choice;
    cout << "1. FCFS\n2. SJT (Non-Preemptive)\n3. SJT (Preemptive)\n4. RR\nChoose a scheduling algorithm to test:";
    cin >> choice;

    switch (choice) {
        case 1:
            cpu.executeFCFS();
            break;
        case 2:
            cpu.executeSJTNonPreemptive();
            break;
        case 3:
            cpu.executeSJTPreemptive();
            break;
        case 4:
            int time_quantum;
            cout << "Enter time quantum for RR:";
            cin >> time_quantum;
            cpu.executeRR(time_quantum);
            break;
        default:
            cerr << "Invalid-Choice1!\n";
            return;
    }

    cpu.displayResults();
}