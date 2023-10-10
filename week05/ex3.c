#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_TH 1000

// primality test
bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}


// Primes counter in [a, b)
int primes_count(int a, int b) {
    int ret = 0;
    for (int i = a; i < b; i++)
        if (is_prime(i) != 0)
            ret++;
    return ret;
}

// argument to the start_routine
// of the thread
typedef struct prime_request {
    int a, b;
} prime_request;

// start_routine of the thread
void *prime_counter(void *arg) {
    // get the request from arg
    prime_request req = *(prime_request *) arg; // perform the request
    int *count = malloc(sizeof(int *));
    *count = primes_count(req.a, req.b);
    return ((void *) count);
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    int res = 0;

    //number of numbers to be checked by each thread
    int div = n / m;

    pthread_t threads[100];
    prime_request requests[100];

    for (int i = 0; i < m; ++i) {
        prime_request request;
        if (i < m - 1) {
            request.a = i * div;
            request.b = (i + 1) * div;
        } else {
            request.a = i * div;
            request.b = n;
        }
        requests[i] = request;
    }

    for (int i = 0; i < m; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, prime_counter, &requests[i]);
        threads[i] = thread;
    }
    int *count = NULL;
    for (int i = 0; i < m; ++i) {
        pthread_join(threads[i], (void **) &count);
        res = res + *count;
        free(count);
    }
    printf("The number of prime numbers is %d\n", res);
    return 0;
}
