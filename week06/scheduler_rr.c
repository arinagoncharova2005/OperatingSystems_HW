#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>

#define PS_MAX 10

#define INP_MAX 100

// holds the scheduling data of one process
typedef struct {
    int idx; // process idx (index)
    int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
    int burst; // remaining burst (this should decrement when the process is being executed)
    int remaining_quantum;
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time = 0; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data
//ready queue
ProcessData ready_queue[PS_MAX];

//index of the last element in queue
int k = 0;

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size = 0;

int quantum;

void read_file(FILE *file) {

    // TODO: extract the data of processes from the {file}
    // and store them in the array {data}
    // initialize ps array to zeros (the process is terminated or not created yet)

    int idx, at, bt;

    //scanning first line of the file "idx at bt"
    char str[INP_MAX];
    fgets(str, sizeof(str), file);

    //scanning numbers from file
    int i = 0;
    while (fgets(str, sizeof(str), file) != NULL) {
        sscanf(str, "%d %d %d", &idx, &at, &bt);
        ProcessData process = {.idx = idx, .at = at, .bt = bt, .rt = 0, .wt = 0, .ct = 0, .tat = 0, .burst = bt, .remaining_quantum = quantum};
        data[i] = process;
        data_size++;
        i++;
    }

    fclose(file);

    //filling array of pids with 0
    for (int j = 0; j < PS_MAX; j++) {
        ps[j] = 0;
    }
}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    // TODO: send signal SIGCONT to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    for (int i = 0; i < sizeof(ps) / sizeof(pid_t); i++) {
        if (ps[i] == process) {
            kill(process, SIGCONT);
            break;
        }
    }
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    // TODO: send signal SIGTSTP to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    for (int i = 0; i < sizeof(ps) / sizeof(pid_t); i++) {
        if (ps[i] == process && ps[i] != 0) {
            //delete pid of the process being stopped
            ps[i] = 0;
            kill(process, SIGTSTP);
            break;
        }
    }

}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    // TODO: send signal SIGTERM to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
    for (int i = 0; i < sizeof(ps) / sizeof(pid_t); i++) {
        if (ps[i] == process) {
            kill(process, SIGTERM);
            break;
        }
    }
}

// create a process using fork
void create_process(int new_process) {

    // TODO: stop the running process
    if (ps[running_process] != 0) {
        suspend(ps[running_process]);
    }

    // TODO: fork a new process and add it to ps array
    int new_process_pid = fork();

    // TODO: The scheduler process runs the program "./worker {new_process}"
    // using one of the exec functions like execvp
    if (new_process_pid == 0) {
        char new_process_str[INP_MAX];
        sprintf(new_process_str, "%d", new_process);
        char *arg[3] = {"./worker", new_process_str, NULL};
        execvp(arg[0], arg);
    } else {
        // TODO: Now the idx of the running process is new_process
        running_process = new_process;
        ps[new_process] = new_process_pid;
    }
}

// find next process for running
ProcessData find_next_process() {

    // location of next process in {data} array
    int location = 0;

    //flag to indicate at the end did we found the next process and to define which process is not arrived yet
    int location_changed = 0;

    for (int i = 0; i < data_size; i++) {

        //process did not start and it arrived
        if (data[i].at == total_time) {
            //add to the ready queue
            ready_queue[k] = data[i];
            k++;
            //data[running_process].remaining_quantum--;
        }
    }

    if (running_process != -1) {
        if (data[running_process].burst>0 && data[running_process].remaining_quantum == 0) {

            ready_queue[k] = data[running_process];
            ready_queue[k].remaining_quantum = quantum;

            k++;

            //delete process from the ready queue
            for (int i = 1; i < k; i++) {
                ready_queue[i - 1] = ready_queue[i];
            }
            k--;
        }
    }

    if (k > 0) {
        location = ready_queue[0].idx;
        location_changed = 1;
    }
    // TODO: find location of the next process to run from the {data} array


    // if next_process did not arrive so far,
    // then we recursively call this function after incrementing total_time
    if (data[location].at > total_time) {

        //if location changed find the process that did not arrived yet
        if (!location_changed) {
            for (int i = 0; i<data_size; i++) {
                if (data[i].at < data[location].at) {
                    location = i;
                }
            }
        }

        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);

        // increment the time
        total_time++;
        return find_next_process();
    }

    return data[location];
}

// reports the metrics and simulation results
void report() {
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i = 0; i < data_size; i++) {
        printf("process %d: \n", i);
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float) sum_wt / data_size;
    float avg_tat = (float) sum_tat / data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}

void check_burst() {

    for (int i = 0; i < data_size; i++)
        if (data[i].burst > 0){
            return;
        }


    // report simulation results
    report();

    // terminate the scheduler :)
    exit(EXIT_SUCCESS);
}


// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum) {
    // increment the total time
    total_time++;

    if (running_process != -1) {
        printf("Scheduler: Runtime: %d seconds\n", total_time);
        data[running_process].burst--;
        data[running_process].remaining_quantum--;
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);
        if (data[running_process].burst == 0) {
            //terminate
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process,
                   data[running_process].burst);
            terminate(ps[running_process]);

            //delete process from the ready queue
            for (int i = 1; i < k; i++) {
                ready_queue[i - 1] = ready_queue[i];
            }
            k--;

            //wait
            wait(&ps[running_process]);

            //calculate metrics
            data[running_process].ct = total_time;
            data[running_process].tat = total_time - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
        }
        check_burst();
        ProcessData next_process = find_next_process();
        if (next_process.idx != running_process) {
            if (ps[running_process] != 0) {
                //running process is still running
                suspend(ps[running_process]);
                printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process,
                       data[running_process].burst);
                running_process = next_process.idx;

            }
            if (ps[running_process] == 0) {
                //creating the process for the next process
                create_process(next_process.idx);
                data[running_process].rt = total_time - data[running_process].at;
                data[running_process].remaining_quantum = quantum;
                printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process,
                       data[running_process].burst);
            } else {
                //the next_process was running and we need to resume it
                resume(ps[next_process.idx]);
                running_process = next_process.idx;
                data[running_process].remaining_quantum = quantum;
                printf("Scheduler: Resuming Process %d (Remaining Time: %d)", running_process,
                       data[running_process].burst);

            }
        }
    } else {
        // find the next process to run
        ProcessData next_process = find_next_process();

        //create a new process for {running_process}
        create_process(next_process.idx);

        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        data[running_process].rt = total_time - data[running_process].at;

    }
}

int main(int argc, char *argv[]) {

    // read the data file
    FILE *in_file = fopen(argv[1], "r");
    printf("Enter the quantum: \n");
    scanf("%d", &quantum);
    //FILE *in_file = fopen("data.txt", "r");
    if (in_file == NULL) {
        printf("File is not found or cannot open it!\n");
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);
    }
    total_time = 0;

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while (1); // infinite loop
}
