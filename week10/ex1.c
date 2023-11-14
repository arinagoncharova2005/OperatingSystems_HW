#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include<unistd.h>

#define PATH_SIZE 1024
#define BUF_SIZE 1024
char* path;

void find_all_hlinks(const char* source) {
    DIR* dir;
    struct dirent* entry;
    char cur_path[PATH_SIZE];
    char path_to_source[PATH_SIZE];
    struct stat info_source;
    struct stat info;
    snprintf(path_to_source, PATH_SIZE, "%s/%s", path, source);
    stat(path_to_source, &info_source);
    long int source_inode = info_source.st_ino;

    if ((dir=opendir(path)) == NULL) {
        perror("Open dir error\n");
    } else {
        while ((entry = readdir(dir))!= NULL) {
            if (entry->d_name[0]!='.') {
                snprintf(cur_path, PATH_SIZE, "%s/%s", path, entry->d_name);
                if (stat(cur_path, &info) != 0) {
                    fprintf(stderr, "stat() error on %s\n", cur_path);
                }
                if (info.st_ino == source_inode && strcmp(entry->d_name, source)!=0) {
                    printf("------------------\n");
                    char buffer_path[PATH_SIZE];
                    realpath(entry->d_name, buffer_path);
                    printf("The inode number: %lu and absolute path is %s\n", info.st_ino, buffer_path);
                    printf("The content of the file: \n");

                    FILE* file = fopen(buffer_path, "r");
                    char buffer[BUF_SIZE];
                    size_t read_bytes;
                    while ((read_bytes = fread(buffer, 1, sizeof(buffer), file))>0) {
                        fwrite(buffer, 1, read_bytes, stdout);
                    }
                    printf("------------------\n");
                    fclose(file);
                }

            }

        }
        closedir(dir);
    }
}

void unlink_all(const char* source) {
    DIR* dir;
    struct dirent* entry;
    char cur_path[PATH_SIZE];
    char path_to_source[PATH_SIZE];
    struct stat info_source;
    struct stat info;
    snprintf(path_to_source, PATH_SIZE, "%s/%s", path, source);
    stat(path_to_source, &info_source);
    long int source_inode = info_source.st_ino;


    if ((dir=opendir(path)) == NULL) {
        perror("Open dir error\n");
    } else {
        //iterating through files
        while ((entry = readdir(dir))!= NULL) {
            if (entry->d_name[0]!='.') {
                snprintf(cur_path, PATH_SIZE, "%s/%s", path, entry->d_name);
                if (stat(entry->d_name, &info) != 0) {
                    fprintf(stderr, "stat() error on %s\n", cur_path);
                }
               
                if (info.st_ino == source_inode && strcmp(cur_path, path_to_source)!=0) {
                    unlink(cur_path);
                    printf("Unlinked %s\n", cur_path);
                }
            }

        }
        closedir(dir);
    }
    printf("Path to the last hard link: %s\n", path_to_source);
}
void create_sym_link(const char* source, const char* link) {
    char link_path[PATH_SIZE];
    snprintf(link_path, PATH_SIZE, "%s/%s", path, link);
    
    symlink(source, link_path);
    printf("Created symbolic link %s for %s\n", link_path, source);
}

void print_info(char* name) {
    char file_path[PATH_SIZE];
    snprintf(file_path, PATH_SIZE, "%s/%s", path, name);
    //printf("PATH TO FILE: %s\n", file_path);
    printf("------------------\n");
    printf("Stat info\n");
    printf("Filename: %s\n", name);

    struct stat file_info;
    if (stat(name, &file_info) == 0) {
        printf("Inode is: %llu\n", (unsigned long long)file_info.st_dev);
        printf("Number of hard links: %hu\n", file_info.st_nlink);
    }
    printf("------------------\n");
}

int main(int argc, char* argv[]) {
    path = argv[1];
    chdir(path);
    FILE * myfile1 = fopen("myfile1.txt", "w");
    fprintf(myfile1, "Hello world.\n");
    fclose(myfile1);

    link("myfile1.txt", "myfile11.txt");
    link("myfile1.txt", "myfile12.txt");


    char new_path1[PATH_SIZE];
    snprintf(new_path1, PATH_SIZE, "%s/%s", path, "myfile11.txt");

    char new_path2[PATH_SIZE];
    snprintf(new_path2, PATH_SIZE, "%s/%s", path, "myfile12.txt");

    rename("myfile11.txt", new_path1);
    rename("myfile11.txt", new_path2);

    find_all_hlinks("myfile1.txt");

    //move myfile1.txt to another folder
    char new_path_myfile1[] = "/tmp/myfile1.txt";
    rename("myfile1.txt", new_path_myfile1);

    FILE* myfile11 = fopen(new_path1, "a");
    fprintf(myfile11, "Modified this file\n");
    fclose(myfile11);

    create_sym_link(new_path_myfile1, "myfile13.txt");

    FILE * my_initial_file = fopen(new_path_myfile1, "a");
    fprintf(my_initial_file, "Modified initial file\n");
    fclose(my_initial_file);

    unlink_all("myfile11.txt");
    print_info("myfile11.txt");
    return 0;
}
