#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "types.h"

// helper function to draw out a row for the timeline chart
void display_timeline(int curr_tick, Core proc_cores[], int core_count, Task task_arr[]) {
    printf("%02d\t", curr_tick);
    for (int idx = 0; idx < core_count; idx++) {
        if (proc_cores[idx].active_task_id != -1) {
            printf("[T%d]\t", task_arr[proc_cores[idx].active_task_id].task_id);
        } else {
            printf("[--]\t");
        }
    }
    printf("\n");
}

// this function grabs the final numbers and displays them after execution is done
void show_stats(Task task_arr[], int total_tasks) {
    printf("\n--- Performance Stats ---\n");
    printf("TID\tArrival\tBurst\tStart\tFinish\tTurnaround\tWaiting\n");
    printf("---\t-------\t-----\t-----\t------\t----------\t-------\n");

    float sum_tat = 0, sum_wt = 0;
    for (int j = 0; j < total_tasks; j++) {
        int tt = task_arr[j].time_done - task_arr[j].time_arrival;
        int wat  = tt - task_arr[j].time_burst;
        sum_tat += tt;
        sum_wt  += wat;
        printf("T%d\t%d\t%d\t%d\t%d\t%d\t\t%d\n",
               task_arr[j].task_id,
               task_arr[j].time_arrival,
               task_arr[j].time_burst,
               task_arr[j].time_start,
               task_arr[j].time_done,
               tt, wat);
    }
    printf("\nMean Turnaround Time : %.2f\n", sum_tat / total_tasks);
    printf("Mean Waiting Time    : %.2f\n", sum_wt  / total_tasks);
}

// lottery scheduling core implementation
// we use random tickets to assign priority
void execute_lottery_sched(Task task_arr[], int total_tasks, int core_count) {
    printf("\n--- Commencing Lottery Scheduling (Quantum = 4) ---\n");
    printf("Tick\t");
    for (int k = 0; k < core_count; k++) {
        printf("Core %d\t", k);
    }
    printf("\n---------------------------------\n");

    Core proc_cores[core_count];
    for (int idx = 0; idx < core_count; idx++) {
        proc_cores[idx].core_id = idx;
        proc_cores[idx].active_task_id = -1;
        proc_cores[idx].quantum_rem  = 0;
    }

    int clock_tick      = 0;
    int done_tasks      = 0;
    int slice_limit     = 4;

    srand((unsigned int)time(NULL));

    while (done_tasks < total_tasks) {

        // first step: select winners from our ready tasks to place on empty cores
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id != -1)
                continue;   // skip busy cores

            // sum up all the tickets of available ready tasks
            int sum_tickets = 0;
            for (int t = 0; t < total_tasks; t++) {
                if (task_arr[t].status == STATUS_READY &&
                    task_arr[t].time_arrival <= clock_tick) {
                    sum_tickets += task_arr[t].lottery_tickets;
                }
            }

            if (sum_tickets == 0)
                continue;   // nobody waiting right now

            // pick a lucky winner based on tickets
            int lucky_pick = (rand() % sum_tickets) + 1;
            int agg_tickets = 0;

            for (int t = 0; t < total_tasks; t++) {
                if (task_arr[t].status == STATUS_READY &&
                    task_arr[t].time_arrival <= clock_tick) {
                    agg_tickets += task_arr[t].lottery_tickets;
                    if (agg_tickets >= lucky_pick) {
                        // send this task to run
                        proc_cores[idx].active_task_id = t;
                        task_arr[t].status  = STATUS_RUN;
                        task_arr[t].processor_id = idx;
                        proc_cores[idx].quantum_rem = slice_limit;

                        if (task_arr[t].time_start == -1) {
                            task_arr[t].time_start = clock_tick;
                        }
                        break;
                    }
                }
            }
        }

        // next step: record this moment in the timeline
        display_timeline(clock_tick, proc_cores, core_count, task_arr);

        // final step: actually run the tasks on the cores for this tick
        for (int idx = 0; idx < core_count; idx++) {
            if (proc_cores[idx].active_task_id == -1)
                continue;

            int active_t = proc_cores[idx].active_task_id;
            task_arr[active_t].time_rem--;
            proc_cores[idx].quantum_rem--;

            // mark as done if remaining time is up
            if (task_arr[active_t].time_rem == 0) {
                task_arr[active_t].status           = STATUS_DONE;
                task_arr[active_t].time_done        = clock_tick + 1;
                proc_cores[idx].active_task_id      = -1;
                done_tasks++;
            }
            // or put back to ready if its time block expired
            else if (proc_cores[idx].quantum_rem == 0) {
                task_arr[active_t].status       = STATUS_READY;
                proc_cores[idx].active_task_id = -1;
            }
        }

        clock_tick++;
    }

    show_stats(task_arr, total_tasks);
}

// rms scheduling core implementation
// priority is completely driven by how short the period interval is
void execute_rms_sched(Task task_arr[], int total_tasks, int core_count) {
    
}