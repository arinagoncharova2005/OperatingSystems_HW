#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FREE_BLOCK_SIZE 128
#define INODE_NUM 16
#define INODE_SIZE 56
#define BYTES_NUM 1024
#define BUF_SIZE 1024
#define INP_MAX 1024
#define FILE_NAME_MAX 256

int fd;

struct inode{
    char name [16]; // file name
    int size; // file size (in number of blocks)
    int blockPointers[8]; // direct block pointers
    int used; // 0 => inode is free; 1 => in use
};

int create(char name[16], int size){
    /*create a file with this name and this size*/

    // Step 1: check to see if we have sufficient free space on disk by
    // reading in the free block list. To do this:
    // move the file pointer to the start of the disk file.
    lseek(fd, 0, SEEK_SET);
    // Read the first 128 bytes (the free/in-use block information)
    char free_block_list[FREE_BLOCK_SIZE];
    read(fd, free_block_list, FREE_BLOCK_SIZE);
    // Scan the list to make sure you have sufficient free blocks to
    // allocate a new file of this size
    int free_blocks_num = 0;
    for (int i =0; i< FREE_BLOCK_SIZE; i++) {
        if (free_block_list[i] == 0) {
            free_blocks_num++;
        }
        if (free_blocks_num>=size) {
            break;
        }
    }
    //there is no enough space
    if (free_blocks_num < size) {
        printf("Insufficient blocks, file with this size cannot be created!\n");
    }


    // Step 2: we look  for a free inode on disk
    // Read in a inode
    // check the "used" field to see if it is free
    // If not, repeat the above two steps until you find a free inode
    // Set the "used" field to 1
    // Copy the filename to the "name" field
    // Copy the file size (in units of blocks) to the "size" field
    struct inode inode;
    int found_inode; //the index of found inode
    for (int i = 0; i<INODE_NUM; i++) {
        lseek(fd, FREE_BLOCK_SIZE + i*INODE_SIZE + 1, SEEK_SET);
        read(fd, &inode, INODE_SIZE);

        // we found free inode
        if (inode.used == 0){
            inode.used = 1;
            strcpy(inode.name, name);
            inode.size = size;
            found_inode = i;
            break;
        }
    }

    // Step 3: Allocate data blocks to the file
    // Scan the block list that you read in Step 1 for a free block
    // Once you find a free block, mark it as in-use (Set it to 1)
    // Set the blockPointer[i] field in the inode to this block number.
    //
    // end for
    for(int i=0;i<size;i++) {
        for(int j = 0; j<FREE_BLOCK_SIZE; j++) {
            if (free_block_list[j] == 0) {
                free_block_list[j] = 1;
                inode.blockPointers[i] = j;
                break;
            }
        }
    }

    // Step 4: Write out the inode and free block list to disk
    //  Move the file pointer to the start of the file
    lseek(fd, 0, SEEK_SET);
    // Write out the 128 byte free block list
    if (write(fd, free_block_list, FREE_BLOCK_SIZE) < 0) {
        printf("Error with write\n");
    }
    // Move the file pointer to the position on disk where this inode was stored
    lseek(fd, FREE_BLOCK_SIZE + found_inode*INODE_SIZE + 1, SEEK_SET);
    // Write out the inode
    write(fd, &inode, INODE_SIZE);
    printf("---------------\n");
    printf("File %s with size %d created\n", name, size);
    printf("---------------\n");
}

int delete(char name[16]){
    // Delete the file with this name
    // Step 1: Locate the inode for this file
    struct inode inode;
    int inode_found_idx = -1; // boolean
    // Move the file pointer to the 1st inode (129th byte)
    for (int i = 0; i < INODE_NUM; i++) {
        lseek(fd, FREE_BLOCK_SIZE + i*INODE_SIZE + 1, SEEK_SET);
        // Read in a inode
        read(fd, &inode, INODE_SIZE);
        // If the inode is free, repeat above step.
        // If the inode is in use, check if the "name" field in the
        // inode matches the file we want to delete. IF not, read the next
        // inode and repeat
        if (inode.used == 1) {
            if (strcmp(inode.name, name) == 0) {
                inode_found_idx = i;
                break;
            }
        }
    }
    // Step 2: free blocks of the file being deleted
    if (inode_found_idx != -1) {
        // Read in the 128 byte free block list (move file pointer to start of the disk and read in 128 bytes)
        lseek(fd, 0, SEEK_SET);
        char freeblockList[FREE_BLOCK_SIZE];
        read(fd, freeblockList, FREE_BLOCK_SIZE);
        // Free each block listed in the blockPointer fields as follows:
        for(int i=0;i< inode.size; i++){
            freeblockList[inode.blockPointers[i]] = 0;
        }
        // Step 3: mark inode as free
        // Set the "used" field to 0.
        inode.used = 0;

        // Step 4: Write out the inode and free block list to disk
        //  Move the file pointer to the start of the file
        lseek(fd, 0, SEEK_SET);
        // Write out the 128 byte free block list
        write(fd, freeblockList, FREE_BLOCK_SIZE);

        // Move the file pointer to the position on disk where this inode was stored
        lseek(fd, FREE_BLOCK_SIZE + inode_found_idx*INODE_SIZE + 1, SEEK_SET);
        write(fd, &inode, INODE_SIZE);
        // Write out the inode
        printf("---------------\n");
        printf("File %s was deleted\n", name);
        printf("---------------\n");
    } else {
        printf("There is no such file\n");
    }

    //  Move the file pointer to the start of the file
    // Write out the 128 byte free block list
    // Move the file pointer to the position on disk where this inode was stored
    // Write out the inode

}

int ls(void){
    // List names of all files on disk

    // Step 1: read in each inode and print!
    // Move file pointer to the position of the 1st inode (129th byte)
    struct inode inode;
    // Read in a inode
    for (int i = 0; i < INODE_NUM; i++) {
        //// TO DO: check+1
        lseek(fd, FREE_BLOCK_SIZE + i * INODE_SIZE + 1, SEEK_SET);
        read(fd, &inode, INODE_SIZE);
        // If the inode is in-use
        // print the "name" and "size" fields from the inode
        if (inode.used == 1) {
            printf("%s %d\n", inode.name, inode.size);
        }
    }
}

int read_block(char name[16], int blockNum, char buf[1024]){

    // read this block from this file
    // Step 1: locate the inode for this file
    // Move file pointer to the position of the 1st inode (129th byte)
    // Read in a inode
    // If the inode is in use, compare the "name" field with the above file
    // IF the file names don't match, repeat
    struct inode inode;
    int inode_found = 0;
    for (int i = 0; i < INODE_NUM; i++) {
        //// TO DO: check+1
        lseek(fd, FREE_BLOCK_SIZE + i * INODE_SIZE + 1, SEEK_SET);
        read(fd, &inode, INODE_SIZE);
        if (inode.used == 1) {
            if (strcmp(inode.name, name) == 0) {
                inode_found = 1;
                break;
            }
        }
    }
    if (inode_found == 0) {
        printf("There is no inode containing information about given file\n");
    } else {
        // Step 2: Read in the specified block
        // Check that blockNum < inode.size, else flag an error
        if (blockNum < inode.size) {
            // Get the disk address of the specified block
            int addr = inode.blockPointers[blockNum];
            // move the file pointer to the block location (i.e., to byte # addr*1024 in the file)
            lseek(fd, addr*BYTES_NUM, SEEK_SET);
            // Read in the block! => Read in 1024 bytes from this location into the buffer "buf"
            read(fd, buf, BYTES_NUM);
            printf("---------------\n");
            printf("Read from file %s from block %d: %s\n", name, blockNum, buf);
            printf("---------------\n");
        } else {
            printf("Error in the blockNum\n");
        }
    }
}

int write_block(char name[16], int blockNum, char buf[1024]){

    // write this block to this file

    // Step 1: locate the inode for this file
    // Move file pointer to the position of the 1st inode (129th byte)
    // Read in a inode
    // If the inode is in use, compare the "name" field with the above file
    // IF the file names don't match, repeat
    struct inode inode;
    int inode_found = 0;
    for (int i = 0; i < INODE_NUM; i++) {
        //// TO DO: check+1
        lseek(fd, FREE_BLOCK_SIZE + i * INODE_SIZE + 1, SEEK_SET);
        read(fd, &inode, INODE_SIZE);
        if (inode.used == 1) {
            if (strcmp(inode.name, name) == 0) {
                inode_found = 1;
                break;
            }
        }
    }

    // Step 2: Write to the specified block
    // Check that blockNum < inode.size, else flag an error
    if (inode_found == 0) {
        printf("There is no inode containing information about given file\n");
    } else {
        // Step 2: Read in the specified block
        // Check that blockNum < inode.size, else flag an error
        if (blockNum < inode.size) {
            // Get the disk address of the specified block
            int addr = inode.blockPointers[blockNum];
            // move the file pointer to the block location (i.e., to byte # addr*1024 in the file)
            lseek(fd, addr*BYTES_NUM, SEEK_SET);
            // Write the block! => Write 1024 bytes from the buffer "buff" to this location
            write(fd, buf, BYTES_NUM);
            printf("---------------\n");
            printf("Buf to file %s at block %d was written: %s\n", name, blockNum, buf);
            printf("---------------\n");
        } else {
            printf("Error in the blockNum\n");
        }
    }
}

char buf[BUF_SIZE];
void generate_buf() {
    //generate buffer for writing
    int char_num = 0;
    char c;
    FILE* random_file = fopen("/dev/random", "r");
    if (random_file == NULL) {
        printf("Error\n");
    }
    while (char_num < BUF_SIZE) {
        fread(&c, 1, 1, random_file);
        if (isprint(c)) {
            buf[char_num] = c;
            char_num++;
        }
    }
    buf[BUF_SIZE-1] = '\0';
    fclose(random_file);
}
int main(int argc, char *argv[]){
    // open input file
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("No input file\n");
        return EXIT_FAILURE;
    }
    
    char str[INP_MAX];
    fgets(str, sizeof(str), input_file);
    // delete '\n' from the end of str
    str[strcspn(str, "\n")] = 0;
    fd = open(str, O_RDWR);
    if (fd < 0) {
        printf("Error with fd\n");
    }

    char command;
    char name_of_file[FILE_NAME_MAX];
    int num;
    while (fgets(str, sizeof(str), input_file)!=NULL) {
        // process string from the input file
        sscanf(str, "%c %s %d", &command, name_of_file, &num);
        //printf(str, "%c %s %d\n", command, name_of_file, num);

        if (command == 'C') {
            create(name_of_file, num);
        } else if (command == 'D') {
            delete(name_of_file);
        } else if (command == 'L') {
            ls();
        } else if (command == 'R') {
            read_block(name_of_file, num, buf);
        } else if (command =='W') {
            generate_buf();
            write_block(name_of_file, num, buf);
        } else {
            printf("There is no such command\n");
        }

    }
    fclose(input_file);
    close(fd);
    return 0;

}
