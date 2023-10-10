#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <string.h>

#define MAX_LEN 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error\n");
        return 0;
    }
    
    printf("Publisher\n");

    int n = atoi(argv[1]);
    int fd;

    //array of pipes
    int fd_pc[4][2];
    mkdir("/tmp/ex1", 0777);
    //creating named fifo
    char fifo[] = "/tmp/ex1/s";


    for (int i = 1; i <= n; i++) {
        pipe(fd_pc[i - 1]);
        pid_t pid = fork();

        char i_str[100];
        sprintf(i_str, "/tmp/ex1/s%d", i);
        mkfifo(i_str, 0666);
        if (pid == 0) {
            //reading message from parent
            char get_message[MAX_LEN];
            close(fd_pc[i-1][1]);
            //writing message to subscriber

            fd = open(i_str, O_WRONLY);
            
            while (1) {

                if (read(fd_pc[i-1][0], get_message, sizeof(get_message))==0) {
                    close(fd_pc[i-1][0]);
                    close(fd);
                    exit(0);
                }
                write(fd, get_message, sizeof(get_message));
            }

        }

    }
    for (int i = 0; i < n; i++) {
        close(fd_pc[i][0]);
    }

    while (1) {
        char read_message[MAX_LEN];
        fgets(read_message, MAX_LEN, stdin);

        //writing message to child
        for (int i = 0; i < n; i++) {
            write(fd_pc[i][1], read_message, sizeof(read_message));
        }
    }

    for (int i = 0; i < n; i++) {
        close(fd_pc[i][1]);
    }

    return 0;
}
