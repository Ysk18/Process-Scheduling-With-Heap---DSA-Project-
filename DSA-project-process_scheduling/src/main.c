#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/scheduler.h"

// Function to print process information
void print_process_info(Process *p)
{
    if (!p)
        return;
    printf("Process ID: %d, Nice: %d, VRuntime: %d, Running: %s\n",
           p->pid,
           p->nice,
           p->vruntime,
           p->is_running ? "Yes" : "No");
}

int main()
{
    // Create a scheduler with capacity for 10 processes
    Scheduler *scheduler = create_scheduler(10);
    if (!scheduler)
    {
        fprintf(stderr, "Failed to create scheduler\n");
        return 1;
    }

    // Create processes with different nice values

    Process processes[] = {
        create_process(1, 1), // Normal priority
        create_process(2, 1), // High priority
        create_process(3, 1), // Low priority
        create_process(4, 1), // Above normal priority
        create_process(5, 1)  // Below normal priority
    };

    // Schedule all processes
    printf("Initial scheduling of processes:\n");
    for (int i = 0; i < 5; i++)
    {
        schedule_process(scheduler, processes[i]);
        printf("Scheduled process %d with nice value %d\n",
               processes[i].pid, processes[i].nice);
    }
    printf("\n");

    // Simulate scheduler running for 50 ticks
    printf("Scheduler simulation for 50 ticks:\n");
    printf("-----------------------------------\n");

    for (int tick_count = 0; tick_count < 50; tick_count++)
    {
        // Get next process to run
        Process *current = get_next_process(scheduler);

        if (current)
        {
            printf("Tick %d: ", tick_count);
            print_process_info(current);
        }
        else
        {
            printf("Tick %d: No process to run\n", tick_count);
            continue;
        }

        // Simulate process execution
        tick(scheduler);

        // Add small delay to make output readable
        usleep(100000); // 100ms delay
    }

    printf("\nSimulation completed\n");

    // Cleanup
    destroy_scheduler(scheduler);
    return 0;
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/scheduler.h"

// Function to print process information

void print_process_info(Process *p)
{
    if (!p)
        return;
    printf("Process ID: %d, Nice: %d, VRuntime: %d, Running: %s\n",
           p->pid,
           p->nice,
           p->vruntime,
           p->is_running ? "Yes" : "No");
}

int main()
{

    // Create a scheduler with capacity for 10 processes
    Scheduler *scheduler = create_scheduler(10);
    if (!scheduler)
    {
        fprintf(stderr, "Failed to create scheduler\n");
        return 1;
    }

    // Create processes with different nice values
    Process processes[] = {
        create_process(1, -20),  // nice: -20
        create_process(2, -19),  // nice: -19
        create_process(3, -18),  // nice: -18
        create_process(4, -17),  // nice: -17
        create_process(5, -16),  // nice: -16
        create_process(6, -15),  // nice: -15
        create_process(7, -14),  // nice: -14
        create_process(8, -13),  // nice: -13
        create_process(9, -12),  // nice: -12
        create_process(10, -11), // nice: -11
        create_process(11, -10), // nice: -10
        create_process(12, -9),  // nice: -9
        create_process(13, -8),  // nice: -8
        create_process(14, -7),  // nice: -7
        create_process(15, -6),  // nice: -6
        create_process(16, -5),  // nice: -5
        create_process(17, -4),  // nice: -4
        create_process(18, -3),  // nice: -3
        create_process(19, -2),  // nice: -2
        create_process(20, -1),  // nice: -1
        create_process(21, 0),   // nice: 0
        create_process(22, 1),   // nice: 1
        create_process(23, 2),   // nice: 2
        create_process(24, 3),   // nice: 3
        create_process(25, 4),   // nice: 4
        create_process(26, 5),   // nice: 5
        create_process(27, 6),   // nice: 6
        create_process(28, 7),   // nice: 7
        create_process(29, 8),   // nice: 8
        create_process(30, 9),   // nice: 9
        create_process(31, 10),  // nice: 10
        create_process(32, 11),  // nice: 11
        create_process(33, 12),  // nice: 12
        create_process(34, 13),  // nice: 13
        create_process(35, 14),  // nice: 14
        create_process(36, 15),  // nice: 15
        create_process(37, 16),  // nice: 16
        create_process(38, 17),  // nice: 17
        create_process(39, 18),  // nice: 18
        create_process(40, 19)   // nice: 19
    };
    int arr[40] = {0};

    // Schedule all processes
    printf("Initial scheduling of processes:\n");
    for (int i = 0; i < 40; i++)
    {
        schedule_process(scheduler, processes[i]);
        printf("Scheduled process %d with nice value %d\n",
               processes[i].pid, processes[i].nice);
    }
    printf("\n");

    // Simulate scheduler running for 50 ticks
    printf("Scheduler simulation for 50 ticks:\n");
    printf("-----------------------------------\n");

    for (int tick_count = 0; tick_count < 10000; tick_count++)
    {
        // Get next process to run
        Process *current = get_next_process(scheduler);

        if (current)
        {
            printf("Tick %d: ", tick_count);
            print_process_info(current);
        }
        else
        {
            printf("Tick %d: No process to run\n", tick_count);
            continue;
        }

        arr[current->pid - 1]++;
        // Simulate process execution
        tick(scheduler);

        // Add small delay to make output readable
        // usleep(100);  // 100ms delay
    }

    printf("\nSimulation completed\n");

    // Cleanup
    destroy_scheduler(scheduler);
    for (int i = 0; i < 40; ++i)
        printf("(%d, %d)\n", processes[i].nice, arr[i]);

    return 0;
}
*/