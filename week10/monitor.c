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

char *path = NULL;
int inotify_fd;
int wd;
struct inotify_event* buffer;

void print_info(char* name) {
    char file_path[PATH_SIZE];
    snprintf(file_path, PATH_SIZE, "%s/%s", path, name);
    
    printf("------------------\n");
    printf("Stat info\n");
    printf("Filename: %s\n", name);
  
    struct stat file_info;
    if (stat(name, &file_info) == 0) {
        printf("Inode is: %llu\n", (long long unsigned)file_info.st_ino);
        printf("Number of hard links: %hu\n", file_info.st_nlink);
    }
    printf("------------------\n");
}
void print_all_stat_info() {
        DIR* dir;
	struct dirent* entry;
	char cur_path[PATH_SIZE];
	struct stat info;
	
	if ((dir=opendir(path)) == NULL) {
	   perror("Open dir error\n");
	} else {
	   while ((entry = readdir(dir))!= NULL) {
	   if (entry->d_name[0]!='.') {
	       snprintf(cur_path, PATH_SIZE, "%s/%s", path,entry->d_name);
	       
	       if (stat(cur_path, &info) == 0) {
	          printf("------------------\n");
	          printf("Filename: %s\n", entry->d_name);
	          printf("Inode number: %llu\n", (long long unsigned)info.st_ino); 
	          printf("Number of hard links: %hu\n", info.st_nlink);
	          printf("------------------\n");
	       }
	       
	   }
	  
           
	   }
	   closedir(dir);
	}
	
}

void handle_sigint(int signal) {
	print_all_stat_info();
	inotify_rm_watch(inotify_fd, wd);
	close(inotify_fd);
	free(buffer);
	exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_sigint);
    path = argv[1];
    chdir(path);
    size_t buf_size = sizeof(struct inotify_event) + NAME_MAX + 1;
    buffer = malloc(buf_size); 
    printf("%s\n", path);
    inotify_fd = inotify_init();
    if (inotify_fd < 0) {
        printf("Error with inotify\n");
    }
    wd = inotify_add_watch(inotify_fd, path, IN_ACCESS|IN_CREATE|IN_DELETE|IN_MODIFY|IN_OPEN|IN_ATTRIB);
    printf("Initial stat info\n");
    print_all_stat_info();
    while (1) {
    
    	int buf_len = read(inotify_fd, buffer, buf_size);
    	
    	int i = 0;
    	
    	while (i<buf_len) {
           struct inotify_event *event = ( struct inotify_event * ) &buffer[i];
           if (event->len) {
           if (event->mask & IN_CREATE) {
            	printf("%s is created\n", event->name);
                print_info(event->name);
            	
           } else if (event->mask & IN_MODIFY) {
            	printf("%s was modified\n", event->name);
                print_info(event->name);
           } else if (event->mask & IN_ACCESS) {
            	printf("%s was accessed\n", event->name);
                print_info(event->name);
           } else if (event->mask & IN_DELETE) {
            	printf("%s deleted from watched directory\n", event->name);
           } else if (event->mask & IN_ATTRIB) {
            	printf("Metadata of %s changed\n", event->name);
                print_info(event->name);
           } else if (event->mask & IN_OPEN) {
            	printf("%s was opened\n", event->name);
                print_info(event->name);
           }
           }
           i = i + sizeof(struct inotify_event) + event->len;
           
    	}
    }

    return 0;
}
