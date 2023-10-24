#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 10000000
#define INP_MAX 1000

unsigned int arr[MAX_SIZE];

void allocate_first_fit(unsigned int adrs, int size) {
    int i = 0;
    //num of free contiguous cells
    int free_num = 0;
    int is_find = 0;

    while (i < MAX_SIZE && !is_find) {
        if (arr[i] == 0 && free_num < size) {
            free_num++;
        } else if (arr[i] != 0) {
            free_num = 0;
        }
        i++;

        if (free_num == size) {
            is_find = 1;
        }
    }
    
    for (int j = i; j > i - size; j--) {
        arr[j - 1] = adrs;
    }
}

void allocate_best_fit(unsigned int adrs, int size) {
    int i = 0;
    int start_idx= 0;
    int cur_start_idx = 0;
    int free_num = 0;
    int free_num_min = MAX_SIZE;

    //find first free cell
    while (i<MAX_SIZE && arr[i] != 0) {
        i++;
        start_idx++;
    }
    cur_start_idx = start_idx;
    while (i < MAX_SIZE) {
        //find the first free block
        if (arr[i] == 0) {
            free_num++;
            i++;
        } else if (arr[i] != 0) {
            if (free_num >= size) {
                if (free_num < free_num_min) {
                    free_num_min = free_num;
                    start_idx = cur_start_idx;
                    free_num = 0;
                }
                free_num = 0;
            } else {
                free_num = 0;
            }
            while (i < MAX_SIZE && arr[i] != 0) {
                i++;
            }
            cur_start_idx = i;
        }
    }
    if (free_num > size && free_num < free_num_min) {
        start_idx = cur_start_idx;
    }

    for (int j= 0; j<size; j++) {
        arr[start_idx+j] = adrs;
    }
}

void allocate_worst_fit(unsigned int adrs, int size) {
    int i = 0;
    int start_idx= 0;
    int cur_start_idx = 0;
    int free_num = 0;
    int free_num_max = 0;

    //find first free cell
    while (i<MAX_SIZE && arr[i] != 0) {
        i++;
        start_idx++;
    }
    cur_start_idx = start_idx;
    while (i < MAX_SIZE) {
        //find the first free block
        if (arr[i] == 0) {
            free_num++;
            i++;
        } else if (arr[i] != 0) {
            if (free_num >= size) {
                if (free_num > free_num_max) {
                    free_num_max = free_num;
                    start_idx = cur_start_idx;
                    free_num = 0;
                }
                free_num = 0;
            } else {
                free_num = 0;
            }
            while (i < MAX_SIZE && arr[i] != 0) {
                i++;
            }
            cur_start_idx = i;
        }
    }
    if (free_num >= size && free_num > free_num_max) {
        start_idx = cur_start_idx;
    }

    for (int j = 0; j<size; j++) {
        arr[start_idx+j] = adrs;
    }
}

void clear(unsigned int adrs) {
    for (int i = 0; i<MAX_SIZE; i++) {
        if (arr[i] == adrs) {
            arr[i] = 0;
        }
    }
}

int main() {
    memset(arr, 0, MAX_SIZE*sizeof(unsigned int));

    FILE *queries = fopen("queries.txt", "r");
    if (queries == NULL){
        printf("Error");
    }
    fflush(stdout);
    char input_line[INP_MAX];
    char cmd[INP_MAX];
    unsigned int adrs;
    int size;

    //first fit
    clock_t start_first_fit = clock();
    int num_of_cmd = 0;
    while (fgets(input_line, sizeof(input_line), queries) != NULL) {
        sscanf(input_line, "%s %d %d",  cmd, &adrs, &size);

        if (strcmp(cmd, "allocate") == 0) {
            allocate_first_fit(adrs, size);
            num_of_cmd++;
        } else if (strcmp(cmd, "clear") == 0){
            clear(adrs);
            num_of_cmd++;
        } else if (strcmp(cmd, "end") == 0) {
            break;
        }
    }

    clock_t end_first_fit = clock();
    double first_time = (double)(end_first_fit - start_first_fit)/CLOCKS_PER_SEC;
    printf("First fit time: %f\n", first_time);
    fclose(queries);
    printf("Throughput first fit: %f\n", num_of_cmd/first_time);

    memset(arr, 0, MAX_SIZE*sizeof(unsigned int));

    queries = fopen("queries.txt", "r");
    //best fit
    clock_t start_best_fit = clock();
    num_of_cmd = 0;
    while (fgets(input_line, sizeof(input_line), queries) != NULL) {
        sscanf(input_line, "%s %d %d",  cmd, &adrs, &size);

        if (strcmp(cmd, "allocate") == 0) {
            allocate_best_fit(adrs, size);
            num_of_cmd++;
        } else if (strcmp(cmd, "clear") == 0){
            clear(adrs);
            num_of_cmd++;
        } else if (strcmp(cmd, "end") == 0) {
            break;
        }
    }
    clock_t end_best_fit = clock();
    double best_time = (double)(end_best_fit - start_best_fit)/CLOCKS_PER_SEC;
    printf("Best fit time: %f\n", best_time);
    fclose(queries);

    printf("Throughput best fit: %f\n", num_of_cmd/best_time);

    memset(arr, 0, MAX_SIZE*sizeof(unsigned int));

    queries = fopen("queries.txt", "r");
    //worst fit
    num_of_cmd = 0;
    clock_t start_worst_fit = clock();
    while (fgets(input_line, sizeof(input_line), queries) != NULL) {
        sscanf(input_line, "%s %d %d",  cmd, &adrs, &size);

        if (strcmp(cmd, "allocate") == 0) {
            allocate_worst_fit(adrs, size);
            num_of_cmd++;
        } else if (strcmp(cmd, "clear") == 0) {
            clear(adrs);
            num_of_cmd++;
        } else if (strcmp(cmd, "end") == 0) {
            break;
        }
    }
    clock_t end_worst_fit = clock();
    double  worst_time = (double)(end_worst_fit - start_worst_fit)/CLOCKS_PER_SEC;
    printf("Worst fit time: %f\n", worst_time);
    printf("Throughput worst fit: %f\n", num_of_cmd/worst_time);

    fclose(queries);
    return 0;
}
