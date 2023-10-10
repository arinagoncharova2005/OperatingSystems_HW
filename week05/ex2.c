#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAX_LEN 256
#define MAX_TH 10000

struct Thread {
    pthread_t id;
    int i;
    char message[MAX_LEN];
};
void *fun(void* arg) {
    struct Thread thread = *(struct Thread*)arg;
    printf("%s\n", thread.message);
    printf("The thread id is: %d\n", (int)thread.id);
    pthread_exit(NULL);
}

int main() {
    int n;
    scanf("%d", &n);
    struct Thread threads[MAX_TH];
    for (int i = 0; i<n; i++) {
        char message[MAX_LEN];
        sprintf(message, "Hello from thread %d", i);
        pthread_t t;
        struct Thread thread = {.id = t, .i = i};
        strcpy(thread.message, message);
        threads[i] = thread;
        pthread_create(&t, NULL, fun, &thread);
        pthread_join(t, NULL);
        printf("Thread %d is created\n", i);
    }
    return 0;
}