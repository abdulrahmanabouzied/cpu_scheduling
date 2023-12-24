#include "pcb.h"

// define the pcb members using scope resolution
// initializing with initialization list
PCB::PCB(int num, int arrival, int burst)
        : process_number(num), arrival_time(arrival), burst_time(burst),
          completion_time(0), turnaround_time(0), waiting_time(0),
          remaining_burst_time(burst), response_time(-1), start_time(0) {};

// operator overloads to compare process struct type
// using reference to the PCB: PCB&
bool operator<(const PCB& p1, const PCB& p2) {
    return p1.arrival_time < p2.arrival_time;
}

bool operator>(const PCB& p1, const PCB& p2) {
    return p1.arrival_time > p2.arrival_time;
}