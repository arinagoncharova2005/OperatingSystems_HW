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

//+1 for termination char
#define STR_SIZE 9
#define F 100
#define P 100

struct PTE {

// The page is in the physical memory (RAM)
    bool valid;
// The frame number of the page in the RAM
    int frame;
// The page should be written to disk
    bool dirty;
// The page is referenced/requested
    int referenced;

};
int num_of_pages;
int num_of_frames;
int num_of_disk_accesses = 0;
struct PTE* mapped_page_table;
int fd_pagetable;
char RAM[F][STR_SIZE];
char disk[P][STR_SIZE];

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
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, mapped_page_table[i].valid, mapped_page_table[i].frame, mapped_page_table[i].dirty, mapped_page_table[i].referenced);
    }
}

void print_separator() {
    printf("-------------------------\n");
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
            found_frame = rand() % num_of_frames;

            
            printf("Chose a random victim page %d\n", found_frame);
            printf("Replace/Evict it with page %d to be allocated to frame %d\n", found_page, found_frame);
                
                //find the page we want to remove from RAM in disk and remove it
                for (int k = 0; k < num_of_pages; k++) {
                    if (mapped_page_table[k].frame == found_frame) {
                        if (mapped_page_table[k].dirty) {
                            strcpy(disk[k], RAM[found_frame]);
                            //update page table
                         
                            mapped_page_table[k].valid = false;
                            mapped_page_table[k].dirty = false;
                            mapped_page_table[k].frame = -1;
                            mapped_page_table[k].referenced = 0;
                            num_of_disk_accesses++;
                            break;
                        }
                        mapped_page_table[k].valid = false;
                        mapped_page_table[k].frame = -1;
                        mapped_page_table[k].referenced = 0;

                    }
                }
        }
        mapped_page_table[found_page].frame = found_frame;
        mapped_page_table[found_page].referenced = 0;
        mapped_page_table[found_page].valid = true;
     

        printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", found_page, found_frame);
        strcpy(RAM[found_frame], disk[found_page]);
        num_of_disk_accesses++;
        print_RAM();
        printf("disk accesses is %d so far\n", num_of_disk_accesses);

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
    srand(time(NULL));
    num_of_pages = atoi(argv[1]);
    num_of_frames = atoi(argv[2]);
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
        struct PTE page_table_entry = {.valid = false, .dirty = false, .frame = -1, .referenced = 0};
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

