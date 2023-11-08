#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>

#define P 1000000
#define TLB_SIZE (int)(0.2*P)
#define PAGE_STR 10

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
struct TLB_entry {
    int page, frame;
};
int num_of_pages;
struct PTE* mapped_page_table;
struct TLB_entry TLB_array[TLB_SIZE];
bool wait_pager = false;

void continue_handler(){
    wait_pager = false;
}
void print_separator() {
    printf("-------------------------\n");
}
void print_page_table() {
    for(int i = 0; i<num_of_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d, counter=%d, aging_counter=%d\n", i, mapped_page_table[i].valid, mapped_page_table[i].frame, mapped_page_table[i].dirty, mapped_page_table[i].referenced, mapped_page_table[i].counter, mapped_page_table[i].aging_counter);
    }
}
void print_TLB() {
    print_separator();
    printf("TLB array\n");
    for(int i = 0; i<num_of_pages*0.2; i++) {
        printf("Page %d ---> frame=%d\n", TLB_array[i].page, TLB_array[i].frame);
    }
    print_separator();
}
int num_of_hits = 0;
int num_of_operations = 0;
int num_of_TLB_miss = 0;
int last_idx_TLB = 0;
void process_operation(char operation, int page_num, pid_t pid) {
    num_of_operations++;
    if (operation == 'R') {
        print_separator();
        printf("Read Request for page %d\n", page_num);
    } else if (operation == 'W') {
        print_separator();
        printf("Write Request for page %d\n", page_num);
    }
    // increment counter
    mapped_page_table[page_num].counter++;
    // update aging counter
    mapped_page_table[page_num].aging_counter = (mapped_page_table[page_num].aging_counter >> 1) | (1 << 7);
    for (int i = 0; i<num_of_pages; i++) {
        if (i != page_num) {
            mapped_page_table[i].aging_counter = (mapped_page_table[i].aging_counter >> 1) | (0 << 7);
        }
    }
    //if the page is in RAM
    if (mapped_page_table[page_num].valid) {
        printf("It is a valid page\n");
        num_of_hits++;
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
	usleep(1000);
        kill(pid, SIGUSR1);
        raise(SIGSTOP);

        printf("MMU resumed by SIGCONT signal from pager\n");

    }
    // check TLB
    bool is_page_in_TLB = false;
    for (int i = 0; i<0.2*num_of_pages; i++) {
        if (TLB_array[i].page == page_num) {
            is_page_in_TLB = true;
            // update frame if it was changed
            TLB_array[i].frame = mapped_page_table[page_num].frame;
            break;
        }
    }
    if (!is_page_in_TLB) {
        num_of_TLB_miss++;
        TLB_array[last_idx_TLB%((int)(num_of_pages * 0.2))].page = page_num;
        printf("Page num %d\n", TLB_array[last_idx_TLB%((int)(num_of_pages * 0.2))].page);
        TLB_array[last_idx_TLB%((int)(num_of_pages * 0.2))].frame = mapped_page_table[page_num].frame;
        last_idx_TLB++;
    }
    print_TLB();

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

    //initialize tlb
    for (int i =0; i<0.2*num_of_pages; i++) {
        struct TLB_entry tlb_entry = {.page = -1, .frame = -1};
        TLB_array[i] = tlb_entry;
    }

    for (int i = 2; i< argc-1; i++) {

        char operation = argv[i][0];
        int cur_arg_size = strlen(argv[i]);
        char page_num_str[PAGE_STR];
        memset(page_num_str, '\0', PAGE_STR);
        for (int j = 1; j<cur_arg_size; j++) {
            page_num_str[j - 1] = argv[i][j];
        }
        int page_num = atoi(page_num_str);
        printf("Page %d\n", page_num);

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
    printf("Number of hits: %d\n", num_of_hits);
    double hit_ratio = (double)num_of_hits/num_of_operations;
    printf("The hit ratio is %f\n", hit_ratio);
    printf("TLB miss ratio: %f\n", (double)num_of_TLB_miss/num_of_operations);
    printf("MMU sends SIGUSR1 to the pager.\n");
    kill(pid, SIGUSR1);
    printf("MMU terminates.\n");

    return 0;
}
