#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 1024

int main() {

    int fd[2];
    pipe(fd);
    pid_t publisher = fork();

    if (publisher == -1) {
        printf("Error\n");
    }
    if (publisher == 0) {
        char message1[MAX_LEN];
        close(fd[0]);
        printf("Enter a message: \n");
        fgets(message1, sizeof(message1), stdin);
        write(fd[1], message1, sizeof(message1));

        close(fd[1]);
        exit(0);
    } else if (publisher > 0) {
        pid_t subscriber = fork();
        if (subscriber == 0) {
            char message2[MAX_LEN];
            close(fd[1]);
            read(fd[0], message2, sizeof(message2));
            printf("The received message is: \n");
            printf("%s", message2);
            close(fd[0]);
            exit(0);
        } else {
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}