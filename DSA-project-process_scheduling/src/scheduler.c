#include <stdlib.h>
#include "../include/scheduler.h"

int compare(const void *a, const void *b)
{
    return ((const Process *)a)->vruntime - ((const Process *)b)->vruntime;
}

/*
 * Creates and initializes a new scheduler instance.
 * The capacity parameter determines the maximum number of processes that can be managed.
 * Returns a pointer to the new scheduler, or NULL if allocation fails.
 */
Scheduler *create_scheduler(int capacity)
{
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));
    if (!scheduler)
        return NULL;
    scheduler->current_process = NULL; //  first set to NULL
    scheduler->process_queue = heap_create(capacity, sizeof(Process), compare);
    if (!scheduler->process_queue)
    {
        free(scheduler);
        return NULL;
    }
    scheduler->time_slice = 1;

    return scheduler;
}

/*
 * Deallocates all resources associated with the scheduler.
 * This includes freeing the process queue, current process, and scheduler structure itself.
 * Should be called when the scheduler is no longer needed to prevent memory leaks.
 */
void destroy_scheduler(Scheduler *scheduler)
{
    if (scheduler)
    {
        if (scheduler->process_queue)
        {
            heap_destroy(scheduler->process_queue);
            scheduler->process_queue = NULL;
        }
        if (scheduler->current_process)
        {
            free(scheduler->current_process);
            scheduler->current_process = NULL;
        }
        free(scheduler);
    }
}

/*
 * Adds a new process to the scheduler's queue.
 * The process is inserted into the min-heap based on its virtual runtime,
 * maintaining the scheduling order where processes with lower virtual runtime
 * have higher priority for execution.
 */
void schedule_process(Scheduler *scheduler, Process process)
{
    if (!scheduler) // if process is null ??
        return;
    heap_insert(scheduler->process_queue, &process);
}

/*
 * Retrieves the next process to be executed based on virtual runtime.
 * Returns the process with the lowest virtual runtime from the queue.
 * If a process is currently running, it is placed back in the queue.
 * Returns NULL if no processes are available for execution.
 */
Process *get_next_process(Scheduler *scheduler)
{
    if (!scheduler || !scheduler->process_queue || scheduler->process_queue->size == 0)
        return NULL;
    if (scheduler->current_process != NULL && scheduler->current_process->is_running) // send back
    {
        scheduler->current_process->is_running = false;

        schedule_process(scheduler, *(scheduler->current_process));
    }

    void *nextProcess = malloc(sizeof(Process)); // create next process that holds top of minHeap
    if (!nextProcess)
        return NULL;

    if (heap_extract_min(scheduler->process_queue, nextProcess))
    {
        free(scheduler->current_process); // free current than Assign this with malloced nextProcess
        scheduler->current_process = (Process *)nextProcess;
        scheduler->current_process->is_running = true;
    }
    else
    {
        free(nextProcess);
        scheduler->current_process = NULL;
    }

    return scheduler->current_process;
}

/*
 * Updates the scheduler state for one time slice unit.
 * Increments the virtual runtime of the currently executing process
 */
void tick(Scheduler *scheduler)
{
    if (scheduler && scheduler->current_process)
        update_vruntime(scheduler->current_process, scheduler->time_slice);
}