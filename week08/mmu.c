#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

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
struct PTE* mapped_page_table;
bool wait_pager = false;

void continue_handler(){
    wait_pager = false;
}
void print_separator() {
    printf("-------------------------\n");
}
void print_page_table() {
    for(int i = 0; i<num_of_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, mapped_page_table[i].valid, mapped_page_table[i].frame, mapped_page_table[i].dirty, mapped_page_table[i].referenced);
    }
}

void process_operation(char operation, int page_num, pid_t pid) {
    if (operation == 'R') {
        print_separator();
        printf("Read Request for page %d\n", page_num);
    } else if (operation == 'W') {
        print_separator();
        printf("Write Request for page %d\n", page_num);
    }
    //if the page is in RAM
    if (mapped_page_table[page_num].valid) {
        printf("It is a valid page\n");
        if (operation == 'W') {
            printf("It is a write request then set the dirty field\n");
            mapped_page_table[page_num].dirty = true;
        }
    }
    if (!mapped_page_table[page_num].valid) {
        printf("It is not a valid page --> page fault\n");
        // If not in RAM
        if (operation == 'W') {
            printf("It is a write request then set the dirty field\n");
            mapped_page_table[page_num].dirty = true;
        }
       
        mapped_page_table[page_num].referenced = getpid();
        printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
       
        kill(pid, SIGUSR1);
        raise(SIGSTOP);

        printf("MMU resumed by SIGCONT signal from pager\n");

    }
 
    print_page_table();

}
int main(int argc, char *argv[]) {

    if (argc == 0){
        printf("No command line arguments\n");
    }
    num_of_pages = atoi(argv[1]);
    char* reference_string = argv[2];
    int reference_string_length = argc - 3;
    pid_t pid = atoi(argv[argc - 1]);


    int fd_pagetable = open("/tmp/ex2/pagetable",  O_RDWR);
    if (fd_pagetable < 0) {
        printf("Error with fd\n");
        return EXIT_FAILURE;
    }

    //truncate file to the size of the page table
    ftruncate(fd_pagetable, sizeof(struct PTE)*num_of_pages);
    mapped_page_table =  mmap(NULL, num_of_pages*sizeof(struct PTE), PROT_READ|PROT_WRITE, MAP_SHARED, fd_pagetable, 0);

    print_separator();
    printf("Initialized page table\n");
    print_page_table();
    print_separator();

    for (int i = 2; i< argc-1; i++) {

        char operation = argv[i][0];
        int page_num = atoi(&argv[i][1]);
        
        //check the validity of input data
        if (page_num >= num_of_pages) {
           printf("There is no such page.\n");
           kill(pid, SIGUSR1);
           return EXIT_FAILURE;
        }

        process_operation(operation, page_num, pid);

    }

    munmap(mapped_page_table,num_of_pages*sizeof(struct PTE));

    close(fd_pagetable);

    printf("Done all requests.\n");
    printf("MMU sends SIGUSR1 to the pager.\n");
    kill(pid, SIGUSR1);
    printf("MMU terminates.\n");

    return 0;
}
