#include <stdio.h>
#include "types.h"

// GANTT CHART PRINTING FUNCTION

void print_gantt_chart(int time, CPU cpus[], int m, Process p[]) {
    printf("%02d\t", time);

    for(int i = 0; i < m; i++) {
        if(cpus[i].current_process_id != -1)
            printf("[P%d]\t", p[cpus[i].current_process_id].id);
        else
            printf("[--]\t"); // idle CPU
    }
    printf("\n");
}


// PERFORMANCE ANALYSIS FUNCTION

void print_analysis(Process p[], int n) {

    printf("\n===== PROCESS ANALYSIS =====\n");
    printf("ID\tAT\tBT\tCT\tTAT\tWT\tRT\n");

    double total_tat = 0, total_wt = 0, total_rt = 0;

    for(int i = 0; i < n; i++) {

        int tat = p[i].completion_time - p[i].arrival_time; // Turnaround Time
        int wt  = tat - p[i].burst_time;                    // Waiting Time
        int rt  = p[i].start_time - p[i].arrival_time;      // Response Time

        total_tat += tat;
        total_wt  += wt;
        total_rt  += rt;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            p[i].id,
            p[i].arrival_time,
            p[i].burst_time,
            p[i].completion_time,
            tat, wt, rt
        );
    }

    // Print averages
    printf("\n===== AVERAGES =====\n");
    printf("Avg Turnaround Time : %.2f\n", total_tat / n);
    printf("Avg Waiting Time    : %.2f\n", total_wt / n);
    printf("Avg Response Time   : %.2f\n", total_rt / n);
}


// ROUND ROBIN (MULTIPROCESSOR)

void run_custom_rr(Process p[], int n, int m) {

    printf("\n--- Round Robin (Quantum = 4) ---\n");

    // Initialize CPUs
    CPU cpus[m];
    for(int i = 0; i < m; i++)
        cpus[i] = (CPU){i, -1, 0};

    int time = 0;
    int completed = 0;
    int quantum = 4;

    // Run until all processes finish
    while(completed < n) {

        // Assign processes to idle CPUs
        for(int i = 0; i < m; i++) {
            if(cpus[i].current_process_id == -1) {

                for(int j = 0; j < n; j++) {
                    if(p[j].state == READY && p[j].arrival_time <= time) {

                        cpus[i].current_process_id = j;
                        p[j].state = RUNNING;
                        p[j].cpu_id = i;
                        cpus[i].time_quantum_left = quantum;

                        // Set response time (first execution)
                        if(p[j].start_time == -1)
                            p[j].start_time = time;

                        break;
                    }
                }
            }
        }

        // Print current execution state
        print_gantt_chart(time, cpus, m, p);

        // Execute one time unit
        for(int i = 0; i < m; i++) {
            if(cpus[i].current_process_id != -1) {

                int id = cpus[i].current_process_id;
                p[id].remaining_time--;
                cpus[i].time_quantum_left--;

                // Process finished
                if(p[id].remaining_time == 0) {
                    p[id].state = FINISHED;
                    p[id].completion_time = time + 1;
                    cpus[i].current_process_id = -1;
                    completed++;
                }
                // Time quantum expired
                else if(cpus[i].time_quantum_left == 0) {
                    p[id].state = READY;
                    cpus[i].current_process_id = -1;
                }
            }
        }

        time++;
    }

    // Print metrics
    print_analysis(p, n);
}


// MULTILEVEL FEEDBACK QUEUE (MLFQ)

void run_mlfq(Process p[], int n, int m) {

    printf("\n--- Multilevel Feedback Queue ---\n");

    CPU cpus[m];
    for(int i = 0; i < m; i++)
        cpus[i] = (CPU){i, -1, 0};

    int time = 0, completed = 0;

    // Quantum for each level
    int quantum[3] = {4, 8, 100000}; // Q2 = FCFS

    while(completed < n) {

        // Assign highest priority process to each CPU
        for(int i = 0; i < m; i++) {

            int best = -1;
            int best_priority = 1e9;

            for(int j = 0; j < n; j++) {
                if(p[j].state == READY && p[j].arrival_time <= time) {

                    if(p[j].priority_level < best_priority) {
                        best_priority = p[j].priority_level;
                        best = j;
                    }
                }
            }

            // Assign if CPU is idle
            if(cpus[i].current_process_id == -1 && best != -1) {
                cpus[i].current_process_id = best;
                p[best].state = RUNNING;
                p[best].cpu_id = i;
                cpus[i].time_quantum_left = quantum[p[best].priority_level];

                if(p[best].start_time == -1)
                    p[best].start_time = time;
            }
        }

        print_gantt_chart(time, cpus, m, p);

        // Execute
        for(int i = 0; i < m; i++) {
            if(cpus[i].current_process_id != -1) {

                int id = cpus[i].current_process_id;
                p[id].remaining_time--;

                // Reduce quantum only for Q0 and Q1
                if(p[id].priority_level < 2)
                    cpus[i].time_quantum_left--;

                // Completion
                if(p[id].remaining_time == 0) {
                    p[id].state = FINISHED;
                    p[id].completion_time = time + 1;
                    cpus[i].current_process_id = -1;
                    completed++;
                }
                // Demotion to lower queue
                else if(p[id].priority_level < 2 && cpus[i].time_quantum_left == 0) {
                    p[id].priority_level++;
                    p[id].state = READY;
                    cpus[i].current_process_id = -1;
                }
            }
        }

        time++;
    }

    print_analysis(p, n);
}