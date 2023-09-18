#include <stdio.h>
#include <string.h>
#include <math.h>

void convert(long long num, int source, int target) {
    int SIZE = 64;
    long long decimal_num = 0;
    long long result=0;

    char num_str[SIZE];
    char reversed_converted_num[SIZE];
    char converted_num[SIZE];
    memset(converted_num,0,sizeof(converted_num));
    sprintf(num_str, "%lld", num);

    //checking input data
    if (source < 2 || target > 10) {
        printf("cannot convert!\n");
        return;
    }
    for (int i = 0; i < strlen(num_str); i++) {
        if (num_str[i] - '0' > source){
            printf("cannot convert!\n");
            return;
        }
    }

    //convert from source to decimal
    int idx = 0;
    while (num != 0) {
        int cur_digit = num % 10;
        decimal_num = decimal_num + cur_digit * pow(source, idx);
        idx++;
        num = num/10;
    }

    //convert from decimal to target
    int i = 0;
    while (decimal_num > 0) {
        int converted_digit = decimal_num % target;
        reversed_converted_num[i] = converted_digit;
        decimal_num = decimal_num / target;
        i++;
    }
    int size_of_res = i;
    for (int j = 0; j < size_of_res; j++) {
        converted_num[j] = reversed_converted_num[i-1]+'0';
        i--;
    }
    sscanf(converted_num, "%lld", &result);
    printf("%lld\n", result);
}


int main() {
    long long num;
    int source;
    int target;

    printf("Enter number, source and target number systems\n");
    scanf("%lld %d %d", &num, &source, &target);

    convert(num, source, target);
    return 0;
}

