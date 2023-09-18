#include <stdio.h>

int main() {
    const int SIZE = 256;
    char str[SIZE];
    char reversed_str[SIZE];

    int i = 0;
    char input_char = 'a';
    while (input_char != '.' && input_char != '\n') {
        input_char = getc(stdin);
        str[i] = input_char;
        i++;
    }

    int k = 0;
    i = i - 2;
    
    // Clear reversed string, otherwise the program prints some garbage after the string    
    for(int j=0;j<SIZE;j++){
        reversed_str[j]=0;
    }
    
    //form reversed string
    for (i; i >= 0; i--) {
        reversed_str[k] = str[i];
        k++;
    }
    
    printf("\"%s\"\n", reversed_str);
    
    return 0;
}

