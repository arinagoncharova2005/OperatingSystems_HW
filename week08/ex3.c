#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>

#define NUM 10

int main() {
    struct rusage usage;

    void*addresses[NUM];
    int i = 1;
    while (i<=NUM) {
        printf("%d\n", i);
        void* memory = malloc(500*1024*1024);
        memset(memory, 0, 500 * 1024 * 1024);
        getrusage(RUSAGE_SELF, &usage);
        printf("Memory: %ld bytes\n", usage.ru_maxrss);
        sleep(1);
        addresses[i - 1] = memory;
        i++;
    }

    for (int j = 0; j<NUM; j++) {
        free(addresses[j]);
    }

    return 0;
}
