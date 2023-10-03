#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 120
#define MAX_P 10

int main() {
    int u[MAX_SIZE];
    int v[MAX_SIZE];
    int n;
    scanf("%d", &n);
    FILE *sharedFile = fopen("temp.txt", "w");

    srand(time(NULL));

    for (int i = 0; i<MAX_SIZE; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    pid_t pid;
    pid_t arr[MAX_P];

    int distr_num = MAX_SIZE / n;

    for (int i = 0; i<n; i++) {
        pid = fork();
        if (pid == 0) {
            arr[i] = pid;
            int res = 0;
            for (int j = i*distr_num; j<(i+1)*distr_num; j++) {
                res = res + u[j]*v[j];
            }
            if (sharedFile != NULL) {
                char res_str[20];
                //convert int to string
                sprintf(res_str, "%d", res);
                //print to file
                fputs(res_str, sharedFile);
                fputs("\n", sharedFile);
            }
            exit(0);
        } else {
            wait(NULL);
        }
    }
    fclose(sharedFile);

    //opening file for reading the results
    sharedFile = fopen("temp.txt", "r");

    int res = 0;

    for (int i = 0; i<n; i++) {
        char line[20];
        fscanf(sharedFile, "%s[ˆ\n]", line);
        int input_num;
        sscanf(line, "%d", &input_num);
        res = res + input_num;
//        printf("%d", input_num);
//        printf("Hello\n");
    }
    fclose(sharedFile);

    //opening file for appending
    sharedFile = fopen("temp.txt", "a");
    char answer[] = "Dot product of vectors u and v is ";
    char res_str[20];
    sprintf(res_str, "%d", res);

    char temp[100];
    strcpy(temp, answer);
    strcat(temp, res_str);
    strcat(temp, "\n");
    fputs(temp, sharedFile);
    fclose(sharedFile);

    //checking
    int another_res = 0;
    for(int i = 0; i<MAX_SIZE; i++) {
        another_res = another_res + u[i]*v[i];
    }
    printf("For checking the dot product is %d\n", another_res);
    return 0;
}
