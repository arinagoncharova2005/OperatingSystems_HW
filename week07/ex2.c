#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define FILE_SIZE 500*1024*1024

int main() {
    FILE* text = fopen("text.txt", "a");
    FILE* random;
    random = fopen("/dev/random", "r");
    if (random == NULL) {
        printf("Error\n");
    }

    char c;

    int num_of_c = 0;
    while (ftell(text) < FILE_SIZE) {
        fread(&c, 1, 1, random);
        //printf("%c\n", c);
        if (isprint(c)) {
            fprintf(text, "%c", c);
            num_of_c++;
        }
        if (num_of_c == 1024) {
            fprintf(text, "%s", "\n");
            num_of_c = 0;
        }
    }
    long int size = ftell(text);

    fclose(text);


    int file = open("text.txt", O_RDWR);

    long sz = sysconf(_SC_PAGESIZE);
    long chunck_size = sz*1024;
    int num_of_capitals = 0;

    int num_of_reads;
    num_of_reads = size/chunck_size;

    for (int i = 0; i<num_of_reads; i++) {
        char* map = mmap(NULL, chunck_size, PROT_READ|PROT_WRITE, MAP_SHARED, file, i*chunck_size);
        if (map == MAP_FAILED){
            printf("Error\n");
        }
        for (int j = 0; j < chunck_size; j++) {
            if (isupper(map[j])) {
                num_of_capitals++;
                map[j] = tolower(map[j]);
            }
        }
        munmap(map, chunck_size);
    }

    close(file);

    printf("Number of capital letters is %d\n", num_of_capitals);
    fclose(random);
    return 0;
}
