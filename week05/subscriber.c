#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LEN 1024

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error\n");
        return 0;
    }

    int id = atoi(argv[1]);
    char fifo[100];
    char message[MAX_LEN];
    sprintf(fifo, "/tmp/ex1/s%d", id);
    int fd = open(fifo, O_RDONLY);
    while (1) {
        int received = read(fd, message, sizeof(message));
        if (received == 0) {
            printf("Stopped!\n");
            break;
        }
        printf("%s\n", message);
    }
    close(fd);

    return 0;
}