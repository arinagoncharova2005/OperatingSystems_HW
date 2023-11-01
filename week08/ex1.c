#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

#define PASS_LEN 8
#define OFFSET 5
#define PASS_LEN_FULL PASS_LEN+OFFSET

int main() {
    FILE* pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file == NULL) {
        printf("Error\n");
        return EXIT_FAILURE;
    }

    pid_t pid = getpid();
    fprintf(pid_file, "%d", pid);
    fclose(pid_file);

    FILE* random_file = fopen("/dev/random", "r");
    if (random_file == NULL) {
        printf("Error\n");
    }
    int num_c = 0;
    char c;
    char pas[PASS_LEN_FULL]="pass:";

    while (num_c<PASS_LEN) {
        fread(&c, 1, 1, random_file);
        if (isprint(c)) {
            pas[num_c+OFFSET] = c;
            num_c++;
        }
    }
    fclose(random_file);

    char* map = mmap(NULL, PASS_LEN_FULL*sizeof(char), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (map == MAP_FAILED){
        printf("Error with map\n");
    }

    for (int i = 0; i< PASS_LEN_FULL; i++) {
        map[i] = pas[i];
    }
    
    printf("%s\n", map);
    while(1) {
        sleep(1);
    }
    munmap(map, PASS_LEN_FULL*sizeof(char));
    return 0;
}

