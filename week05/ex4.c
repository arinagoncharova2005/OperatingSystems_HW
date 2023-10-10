#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_TH 1000

// primality test (from ex3)
bool is_prime(int n) {
    if (n <= 1) return false;
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// The mutex
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

// Do not modify these variables directly , use the functions on the right side.
int k = 0;
int c = 0;

// input from command line
int n = 0;

// get next prime candidate
int get_number_to_check() {
    int ret = k;
    if (k != n)
        k++;
    return ret;
}

// increase prime counter
void increment_primes() {
    c++;
}

// start_routine
void *check_primes(void *arg) {
    while (1){
        pthread_mutex_lock(&global_lock);
        int num = get_number_to_check();
        pthread_mutex_unlock(&global_lock);

        if (num == n) {
            break;
        }

        if (is_prime(num)) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_mutex_init(&global_lock, NULL);
    pthread_t threads[NUM_TH];

    for(int i = 0; i<m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);

    }

    for(int i = 0; i<m; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("The number of prime numbers is %d\n", c);

    pthread_mutex_destroy(&global_lock);
    return 0;
}