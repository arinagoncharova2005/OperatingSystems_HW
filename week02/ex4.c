#include <stdio.h>
#include <string.h>
#include <ctype.h>


int count(char string[], char c) {
    int num_of_char = 0;
    for (int i = 0; i< strlen(string); i++) {
        if (tolower(string[i]) == tolower(c)){
            num_of_char++;
        }
    }
    return num_of_char;
}
void countAll(char string[]) {
    int str_size = strlen(string) - 1;

    for (int i = 0; i < str_size; i++) {
        count(string, string[i]);
        if (i == str_size - 1) {
            printf("%c: %d\n", tolower(string[i]), count(string, string[i]));
        } else {
            printf("%c: %d, ", tolower(string[i]), count(string, string[i]));
        }
    }
}
int main() {
    const int MAX_SIZE = 256;
    char string[MAX_SIZE];

    fgets(string, MAX_SIZE, stdin);

    countAll(string);

    return 0;
}

