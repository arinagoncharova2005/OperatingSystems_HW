#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>

//+1 for termination char
#define STR_SIZE 9
#define F 1000000
#define P 1000000
#define ALG_LEN 10

struct PTE {

// The page is in the physical memory (RAM)
    bool valid;
// The frame number of the page in the RAM
    int frame;
// The page should be written to disk
    bool dirty;
// The page is referenced/requested
    int referenced;
    int counter;
    unsigned char aging_counter;

};

int num_of_pages;
int num_of_frames;
int num_of_disk_accesses = 0;
struct PTE* mapped_page_table;
int fd_pagetable;
char RAM[F][STR_SIZE];
char disk[P][STR_SIZE];
char replacement_algorithm[ALG_LEN];


void print_RAM() {
    printf("RAM array\n");
    for (int i = 0; i<num_of_frames; i++) {
        printf("Frame %d ---> %s\n", i, RAM[i]);
    }
}

void print_disk() {
    printf("Disk array\n");
    for (int i = 0; i<num_of_pages; i++) {
        printf("Page %d ---> %s\n", i, disk[i]);
    }
}

void print_page_table() {
    for(int i = 0; i<num_of_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, counter=%d, aging_counter=%d\n", i, mapped_page_table[i].valid, mapped_page_table[i].frame, mapped_page_table[i].dirty, mapped_page_table[i].referenced, mapped_page_table[i].counter, mapped_page_table[i].aging_counter);
    }
}

void print_separator() {
    printf("-------------------------\n");
}
// Random page replacement
int my_random(struct PTE* page_table){
    //search in the page table randomly to find a valid page.
    // When you evict the page, you reset its entry in the page table and you update the entry of the page with the frame
    // that was reserved for the evicted page.
    int found_random_page_in_pt = rand() % num_of_pages;
    //check if the found random page is in the RAM
    while (!page_table[found_random_page_in_pt].valid) {
        found_random_page_in_pt = rand() % num_of_pages;
    }
    return found_random_page_in_pt;
}

// NFU page replacement
int nfu(struct PTE* page_table) {
    int min_counter = INT_MAX;
    // page number with the minimum counter
    int evict_page_number = -1;
    for (int i = 0; i<num_of_pages; i++) {
        if (page_table[i].counter < min_counter && page_table[i].valid) {
            min_counter = page_table[i].counter;
            evict_page_number = i;
        }
    }
    //printf("FOUND PAGE NUM: %d, FRAME: %d\n", evict_page_number, mapped_page_table[evict_page_number].frame);
    return evict_page_number;
}
// Aging page replacement
int aging(struct PTE* page_table) {
    unsigned char min_aging_counter = UCHAR_MAX;
    //page number with the minimum aging counter
    int evict_page_number = -1;
    for (int i = 0; i<num_of_pages; i++) {
        if (page_table[i].aging_counter < min_aging_counter && page_table[i].valid) {
            min_aging_counter = page_table[i].aging_counter;
            evict_page_number = i;
        }
    }
    //printf("FOUND PAGE NUM: %d, FRAME: %d\n", evict_page_number, mapped_page_table[evict_page_number].frame);
    return evict_page_number;
}

void handle_signal1(){
    print_separator();
    //page that we need to load to RAM
    int found_page = -1;


    for (int i = 0; i<num_of_pages; i++) {
        if (mapped_page_table[i].referenced != 0) {
            found_page = i;
            break;
        }
    }


    if (found_page != -1) {
        pid_t mmu_pid = mapped_page_table[found_page].referenced;
        printf("A disk access request from MMU Process (pid=%d)\n", mmu_pid);
        printf("Page %d is referenced\n", found_page);
        int found_frame = -1;
        for (int j = 0; j < num_of_frames; j++) {
            if (strlen(RAM[j]) == 0) {
                found_frame = j;
                break;
            }
        }
        if (found_frame != -1) {
            printf("We can allocate it to free frame %d\n", found_frame);
        }
        if (found_frame == -1) {
            //do not find free frame, find random frame
            printf("We do not have free frames in RAM\n");
            int found_evict_page;

            //find the page we want to remove from RAM in disk and remove it
            if (strcmp(replacement_algorithm, "random") == 0) {
                 found_evict_page = my_random(mapped_page_table);
                 found_frame = mapped_page_table[found_evict_page].frame;
            } else if (strcmp(replacement_algorithm, "nfu") == 0) {
                found_evict_page = nfu(mapped_page_table);
                found_frame = mapped_page_table[found_evict_page].frame;
            } else if (strcmp(replacement_algorithm, "aging") == 0) {
                found_evict_page = aging(mapped_page_table);
                found_frame = mapped_page_table[found_evict_page].frame;
            } else {
                printf("Error with page replacement algorithm!");
            }

            printf("Chose a victim page %d\n", found_evict_page);
            printf("Replace/Evict it with page %d to be allocated to frame %d\n", found_page, found_frame);

            //if evict page is dirty
            if (mapped_page_table[found_evict_page].dirty) {
                // copying changed info to disk
                strcpy(disk[found_evict_page], RAM[found_frame]);
                mapped_page_table[found_evict_page].dirty = false;
                num_of_disk_accesses++;
            }

            // remove evict_page from RAM
            mapped_page_table[found_evict_page].valid = false;
            mapped_page_table[found_evict_page].frame = -1;
            mapped_page_table[found_evict_page].referenced = 0;
            mapped_page_table[found_evict_page].counter = 0;
            mapped_page_table[found_evict_page].aging_counter = 0;

        }
        // load referenced page to RAM
        mapped_page_table[found_page].frame = found_frame;
        mapped_page_table[found_page].referenced = 0;
        mapped_page_table[found_page].valid = true;


        printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", found_page, found_frame);
        strcpy(RAM[found_frame], disk[found_page]);
        num_of_disk_accesses++;
        print_RAM();
        printf("disk accesses is %d so far\n", num_of_disk_accesses);
        
        usleep(1000);

        kill(mmu_pid, SIGCONT);
    }

    if (found_page == -1){

        printf("%d disk accesses in total\n", num_of_disk_accesses);
        printf("Pager is terminated\n");
        close(fd_pagetable);
        munmap(mapped_page_table, sizeof(struct PTE)*num_of_pages);
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[]) {
    printf("Pager pid: %d\n", getpid());
    srand(time(NULL));
    num_of_pages = atoi(argv[1]);
    num_of_frames = atoi(argv[2]);
    strcpy(replacement_algorithm, argv[3]);
    printf("Selected page replacement algorithm: %s\n", replacement_algorithm);
    signal(SIGUSR1, handle_signal1);

    if (num_of_frames > F) {
        printf("Incorrect number of frames\n");
        return EXIT_FAILURE;
    }

    if (num_of_pages > P) {
        printf("Incorrect number of pages\n");
        return EXIT_FAILURE;
    }

    //creating and initializing the page table
    mkdir("/tmp/ex2", 0777);
    fd_pagetable = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT);
    if (fd_pagetable < 0) {
        printf("Error with fd\n");
        return 0;
    }
    //truncate file to the size of the page table
    ftruncate(fd_pagetable, sizeof(struct PTE)*num_of_pages);

    mapped_page_table =  mmap(NULL, num_of_pages*sizeof(struct PTE), PROT_READ|PROT_WRITE, MAP_SHARED, fd_pagetable, 0);
    for(int i =0; i<num_of_pages; i++) {
        struct PTE page_table_entry = {.valid = false, .dirty = false, .frame = -1, .referenced = 0, .counter=0, .aging_counter = 0};
        mapped_page_table[i] = page_table_entry;
    }
    print_separator();
    printf("Initialized page table\n");
    print_page_table();
    print_separator();

    //filling RAM with ""
    for (int i = 0; i<num_of_frames; i++) {
        strcpy(RAM[i], "");
    }
    printf("Initialized RAM\n");
    print_RAM();
    print_separator();

    //filling disk
    char c;
    for (int i = 0; i<num_of_pages; i++) {
        for (int j = 0; j<STR_SIZE - 1; j++) {
            c = (char)('a' + rand()%(26));
            disk[i][j] = c;
        }
        disk[i][STR_SIZE-1] = '\0';
    }
    printf("Initialized disk\n");
    print_disk();
    print_separator();
    while(1) {
        sleep(1);
    }

    return 0;

}
