#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    clock_t p1_begin, p2_begin, main_begin;
    clock_t p1_end, p2_end, main_end;
    main_begin = clock();

    pid_t pid1 = fork();
    p1_begin = clock();
    //if child
    if (pid1 == 0) {
        printf("I am the 1st child process with id: %d and my parent's id: %d\n", getpid(), getppid());

        p1_end = clock();
        printf("Execution time of the 1st process is %f\n", (double)(p1_end - p1_begin)/CLOCKS_PER_SEC * 1000);
        printf("\n");

        exit(0);
    } else if (pid1>0){
        //create the second child
        pid_t pid2 = fork();
        p2_begin = clock();
        if (pid2 == 0) {
            printf("I am the 2nd child process with id: %d and my parent's id: %d\n", getpid(), getppid());

            p2_end = clock();
            printf("Execution time of the 2nd process is %f\n", (double)(p2_end - p2_begin)/CLOCKS_PER_SEC * 1000);
            printf("\n");

            exit(0);
        } else if (pid2>0) {
            printf("I am a parent process with id: %d\n", getpid());
            main_end = clock();
            printf("Execution time of the main (parent) process is %f\n", (double)(main_end - main_begin)/CLOCKS_PER_SEC * 1000);
            printf("\n");
            wait(NULL);
            wait(NULL);
        } else {
            printf("The second process cannot be created\n");
        }

    } else {
        printf("The first process cannot be created\n");
    }
    return 0;
}
