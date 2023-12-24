#ifndef CPU_PCB_H
#define CPU_PCB_H

// to use pid_t #include <sys/types.h>

/**
 * PCB - a struct defined to wrap the process-related data.
 * @PCB: constructor to creat a process control block
 * @process_numer: pid.
 * @arrival_time: process arrival, may arrive many times if muted.
 * @start_time: first arrival.
 * @burst_time: processing time.
 * @completion_time: completed at.
 * @turnaround_time: waiting + burst.
 * @waiting_time: for processing.
 * @remaining_burst_time: if muted.
 * @response_time: time between arrival and first response to it in the cpu.
 */
struct PCB {
    int process_number;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int remaining_burst_time;
    int response_time;
    int start_time; // Starting arrival time

    // Constructor to initialize PCB
    PCB(int num, int arrival, int burst);
};

// Operator overloads for PCB
// comparing the arrival times
bool operator<(const PCB& p1, const PCB& p2);
bool operator>(const PCB& p1, const PCB& p2);

#endif //CPU_PCB_H
