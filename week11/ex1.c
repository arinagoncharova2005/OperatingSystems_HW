#include <stdio.h>
#include <string.h>
#include <dirent.h>

void list_root_content() {
    DIR* dir;
    struct dirent* entry;
    char path[] = {"/"};
    if ((dir=opendir(path)) == NULL) {
        perror("Open dir error\n");
    } else {
        // go through all the files placed in the root directory
        // and display their names
        while ((entry = readdir(dir))!= NULL) {
            if (entry->d_name[0]!='.') {
                printf("%s\n", entry->d_name);
            }
        }
        closedir(dir);
    }
}

int main() {
    list_root_content();
    return 0;
}

