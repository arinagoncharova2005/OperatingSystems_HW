#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_STR 255


int main() {
    char input_cmd[MAX_STR];
    while (1){
        fgets(input_cmd, sizeof(input_cmd), stdin);
        char *token = NULL;
        input_cmd[strcspn(input_cmd, "\n")] = '\0';
        token = strtok(input_cmd, " ");

        int num_inp = 0;
        char* argv[MAX_STR];
        while (token != NULL) {
            argv[num_inp] = malloc((sizeof (token)));
            strcpy(argv[num_inp++], token);
            token = strtok(NULL, " ");
        }

        if (argv[num_inp-1][0] == '&') {
            argv[num_inp-1] = NULL;
            pid_t new_pid = fork();
            if (new_pid == 0) {
                execvp(argv[0], argv);
            }
        } else {
            argv[num_inp] = NULL;
            pid_t pid = fork();
            if (pid == 0) {
                execvp(argv[0], argv);
            } else {
                wait(NULL);
            }
        }
    }

    return 0;
}
