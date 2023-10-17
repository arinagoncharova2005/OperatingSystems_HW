#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define MAX_LEN 100



void read_signal() {
    FILE* text_file = fopen("text.txt", "r");
    char message[MAX_LEN];
    fgets(message, MAX_LEN, text_file);
    printf("%s\n", message);
    fclose(text_file);
}

void exit_signal() {
    kill(getpid(), SIGTERM);
    //exit(0);
}

int main() {
    FILE* file_pid = fopen("/var/run/agent.pid", "w");
    //FILE* file_pid = fopen("agent.pid", "w");
    //text_file = fopen("text.txt", "r");

    char message[MAX_LEN];

    signal(SIGUSR1, read_signal);
    signal(SIGUSR2, exit_signal);

    int pid = getpid();
    fprintf(file_pid, "%d\n", pid);
    //fflush(file_pid);
    fclose(file_pid);

    read_signal();
    //fclose(text_file);

//    fgets(message, MAX_LEN, text_file);
//    printf("%s\n", message);

    while (1) {
        sleep(1);
    }


    return 0;
}

