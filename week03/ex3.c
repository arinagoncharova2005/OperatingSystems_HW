#include <stdio.h>
#include <string.h>

#define MAX_PATH 2048
#define FILE_NAME_SIZE 64
#define PATH_SIZE 2048
#define FILE_DATA_SIZE 1025
#define MAX_NUM 255

typedef struct Directory Directory;
typedef struct File File;
struct Directory {
    char name[FILE_NAME_SIZE];
    struct File* files[MAX_NUM];
    struct Directory* directories[MAX_NUM];
    unsigned char nf;
    unsigned char nd;
    char path[PATH_SIZE];
};

struct File {
    int id;
    char name[FILE_NAME_SIZE];
    int size;
    char data[FILE_DATA_SIZE];
    struct Directory* directory;
};

//Prints the name of the File file
void show_file(File *file) {
    printf("%s ", file->name);
}

// Displays the content of the Directory dir
void show_dir(Directory* dir) {
    printf(" \nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < dir->nf; i++) {
        show_file(dir->files[i]);
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < dir->nd; i++) {
        show_dir(dir->directories[i]);

    }
    printf("}\n");
}

// Adds the subdirectory dir1 to the directory dir2
void add_dir(Directory *dir1, Directory *dir2) {
    if (dir1 && dir2) {
        dir2->directories[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[MAX_PATH];
        if (strcmp(dir2->path, "/")) {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        } else {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}

void overwrite_to_file(struct  File* file, const char* str) {
    strcpy(file->data, str);
    file->size = strlen(file->data);
}

void append_to_file(struct  File* file, const char* str) {
    if (file->size + strlen(str) < FILE_DATA_SIZE) {
        strcat(file->data, str);
        file->size = strlen(file->data);
    }
}

void printp_file(struct File* file) {
    char temp_path[MAX_PATH];
    struct Directory* current_dir = file->directory;
    strcpy(temp_path, current_dir->path);
    strcat(temp_path, "/");
    strcat(temp_path, file->name);
    printf("%s\n", temp_path);
}

void add_file(struct File* file, struct Directory* dir) {
    if (dir->nd+1< MAX_NUM) {
        dir->files[dir->nf] = file;
        dir->nf++;
        file->directory = dir;
    }
}

int main() {
    struct Directory root = {.path = "/", .name = "root", .nd = 0, .nf = 0, .files = {}, .directories = {}};
    struct Directory home = {.path = "", .name = "home", .nd = 0, .nf = 0, .files = {}, .directories = {}};
    struct Directory bin = {.path = "", .name = "bin", .nd = 0, .nf = 0, .files = {}, .directories = {}};
    add_dir(&home, &root);
    add_dir(&bin, &root);

    struct File bash = {.name = "bash", .data = "", .size = 0, .id = 1};
    add_file(&bash, &bin);

    struct File ex3_1 = {.name = "ex3_1.c", .data = "int printf(const char * format, ...);", .size = strlen(ex3_1.data), .id = 2};
    struct File ex3_2 = {.name = "ex3_2.c", .data = "//This is a comment in C language", .size = strlen(ex3_2.data), .id = 3};
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);

    char* str = "Bourne Again Shell!!";
    overwrite_to_file(&bash, str);

    char* content = "int main(){printf(”Hello World!”)}";
    append_to_file(&ex3_1, content);

    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);

    return 0;
}
