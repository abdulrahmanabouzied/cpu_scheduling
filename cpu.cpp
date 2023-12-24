#include "cpu.h"

void CPU::takeNewProcess(int process_number, int arrival_time, int burst_time) {
    // emplace_back creates new PCB with this data and push it
    ready_queue.emplace_back(process_number, arrival_time, burst_time);
}

void CPU::executeFCFS() {
    deque<PCB> fcfs_queue(ready_queue.begin(), ready_queue.end()); // Use deque instead

    // to make arrival is the order key, comparable with its overload
    sort(fcfs_queue.begin(), fcfs_queue.end());

    while (!fcfs_queue.empty()) {
        PCB current_process = fcfs_queue.front();
        fcfs_queue.pop_front();

        // Execute the process
        // the start time equals the arrival if it is the first process comes,
        // or its arrival is greater than the prev completion
        // otherwise, it is the previous process completion time
        current_process.start_time = max(current_process.arrival_time, completion_queue.empty()
                                                                       ? 0
                                                                       : completion_queue.back().completion_time);

        current_process.completion_time = current_process.start_time + current_process.burst_time;
        current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
        current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;

        completion_queue.push_back(current_process);
    }
}

// a comparator struct required to sort the priority queue by the burst time
struct SJTComparator {
    bool operator()(const PCB& p1, const PCB& p2) const {
        // Define your custom comparison logic here
        // For example, compare by burst time
        return p1.burst_time > p2.burst_time;
    }
};


void CPU::executeSJTNonPreemptive() {
    // change made greater<PCB>
    priority_queue<PCB, vector<PCB>, SJTComparator> sjt_queue(ready_queue.begin(), ready_queue.end());

    while (!sjt_queue.empty()) {
        PCB current_process = sjt_queue.top();
        sjt_queue.pop();

        // Execute the process
        current_process.start_time = max(current_process.arrival_time, completion_queue.empty()
                                                                       ? 0
                                                                       : completion_queue.back().completion_time);

        current_process.completion_time = current_process.start_time + current_process.burst_time;
        current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
        current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;

        completion_queue.push_back(current_process);
    }
}

void CPU::executeSJTPreemptive() {
    vector<PCB> sjt_processes(ready_queue.begin(), ready_queue.end());
    sort(sjt_processes.begin(), sjt_processes.end(), [](const PCB& p1, const PCB& p2) {
        return p1.burst_time < p2.burst_time;
    });

    int current_time = 0;

    while (!sjt_processes.empty()) {
        // find the process with the shortest remaining burst time
        // using lambda function [capture](params)->return_type{//body}
        auto it = min_element(sjt_processes.begin(), sjt_processes.end(),
                              [current_time](const PCB& p1, const PCB& p2) {
                                  return p1.remaining_burst_time < p2.remaining_burst_time &&
                                         p1.arrival_time <= current_time;
                              });

        // if case matched that -> process arrived, if many choose the min burst
        // if it === end(), no match -> empty
        if (it != sjt_processes.end()) {
            // reference to the PCB object from the vector
            PCB& current_process = *it;

            // if the arrival time is greater than the curr, then it is the start
            // otherwise the curr time is the start
            // suppose arrival is 1 so can't start it in 0
            current_process.start_time = max(current_process.arrival_time, current_time);

            // we used units to represent time
            // every unit of time we check if there is a process with minimum remaining burst
            // than the curr one
            int burst_time = min(current_process.remaining_burst_time, 1);
            current_time += burst_time;
            current_process.remaining_burst_time -= burst_time;

            // if the process had been finished it popped up and calc its attributes
            if (current_process.remaining_burst_time == 0) {
                current_process.completion_time = current_process.start_time + current_process.burst_time;
                current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
                current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;

                completion_queue.push_back(current_process);
                // remove the process object from curr ready_queue
                // to use it again
                sjt_processes.erase(it);
            }

            // change: removed else clause
        } else {
            // arrival time not reached yet
            current_time++;
        }
    }
}

void CPU::executeRR(int time_quantum) {
    deque<PCB> rr_queue(ready_queue.begin(), ready_queue.end());

    sort(rr_queue.begin(), rr_queue.end());

    while (!rr_queue.empty()) {
        PCB current_process = rr_queue.front();
        rr_queue.pop_front();

        // Execute the process
        current_process.start_time = max(current_process.arrival_time, completion_queue.empty()
                                                                       ? 0
                                                                       : completion_queue.back().completion_time);

        int burst_time = min(current_process.remaining_burst_time, time_quantum);
        current_process.remaining_burst_time -= burst_time;

        if (current_process.remaining_burst_time == 0) {
            current_process.completion_time = current_process.start_time + current_process.burst_time;
            current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
            current_process.waiting_time = current_process.turnaround_time - current_process.burst_time;

            completion_queue.push_back(current_process);
        } else {
            current_process.arrival_time = current_process.start_time + time_quantum;
            rr_queue.push_back(current_process);
        }
    }
}

void CPU::displayResults() {
    cout << "pid\tWT\tCT\tTAT\n";
    for (const auto& process : completion_queue) {
        cout << process.process_number << "\t" << process.waiting_time << "\t"
             << process.completion_time << "\t" << process.turnaround_time << "\n";
    }

    int total_waiting_time = 0, total_completion_time = 0, total_turnaround_time = 0;
    for (const auto& process : completion_queue) {
        total_waiting_time += process.waiting_time;
        total_completion_time += process.completion_time;
        total_turnaround_time += process.turnaround_time;
    }

    int num_processes = (int)completion_queue.size();

    cout << "\nAverage Waiting Time: " << calculateAverage(total_waiting_time, num_processes) << "\n";
    cout << "Average Completion Time: " << calculateAverage(total_completion_time, num_processes) << "\n";
    cout << "Average Turnaround Time: " << calculateAverage(total_turnaround_time, num_processes) << "\n";
}

void CPU::showState() {
    cout << "Processes:\n";
    cout << "PID\tAT\tBT\tST\n";

    for (const PCB& process : ready_queue) {
        cout << process.process_number << "\t"
                  << process.arrival_time << "\t"
                  << process.burst_time << "\t"
                  << process.start_time << "\n";
    }

    cout << "\n";
    for (const PCB& process : completion_queue) {
        cout << process.process_number << "\t"
                  << process.arrival_time << "\t"
                  << process.burst_time << "\t"
                  << process.start_time << "\n";
    }

    cout << "\n";
}

double CPU::calculateAverage(int sum, int count) {
    return static_cast<double>(sum) / count;
}

// get user data
void CPU::getData() {
    int process_number, arrival_time, burst_time;
    int more_processes;

    do {
        cout << "Enter process details:\n";
        cout << "Process Number:";
        cin >> process_number;
        cout << "Arrival Time:";
        cin >> arrival_time;
        cout << "Burst Time:";
        cin >> burst_time;

        // Add the entered process to the ready queue
        takeNewProcess(process_number, arrival_time, burst_time);

        cout << "Enter more processes? (1/0):";
        cin >> more_processes;
    } while (more_processes == 1);

    showState();
}