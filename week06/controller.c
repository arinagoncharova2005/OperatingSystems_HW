#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 100

int agent_pid;

void terminate(){
    kill(agent_pid, SIGTERM);
    exit(0);
}

int main() {
    FILE* agent_pid_file;

    signal(SIGINT, terminate);

    agent_pid_file = fopen("/var/run/agent.pid", "r");
    fscanf(agent_pid_file, "%d", &agent_pid);
    if (getpgid(agent_pid) >= 0) {
        printf("Agent found %d\n", agent_pid);
    } else {
        printf("Error: No agent found.\n");
        return 0;
    }
    while (1) {
        printf("Choose a command {“read”, “exit”, “stop”, “continue”} to send to the agent\n");
        char command[MAX_LEN];
        scanf("%s", command);
        if (strcmp(command, "read") == 0) {
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(agent_pid, SIGUSR2);
            exit(0);
        } else if (strcmp(command, "stop") == 0) {
            kill(agent_pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(agent_pid, SIGCONT);
        } else {
            printf("No command\n");
        }
    }

    return 0;
}
