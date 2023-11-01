#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>

#define NUM 10
#define MEM_SIZE 10*1024*1024

int main() {
    struct rusage usage;

    char*addresses[NUM];
    int i = 1;
    while (i<=NUM) {
        printf("%d\n", i);
        char* memory = malloc(MEM_SIZE);
        memset(memory, 0, MEM_SIZE);
        getrusage(RUSAGE_SELF, &usage);
        printf("Memory: %ld KB\n", usage.ru_maxrss);
        sleep(1);
        addresses[i - 1] = memory;
        i++;
    }

    for (int j = 0; j<NUM; j++) {
        free(addresses[j]);
    }

    return 0;
}
