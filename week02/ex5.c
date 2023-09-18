#include <stdio.h>

long long tribonacci(int num) {
    int first = 0;
    int second = 1;
    int third = 1;
    if (num == 0) {
        printf("%d", first);
    } else if (num == 2){
        printf("%d", second);
    } else {
        for (int i = 3; i<=num; i++) {
            int new_num = first + second + third;
            first = second;
            second = third;
            third = new_num;
        }
    }
    return  third;
}

int main() {
    int a = 4;
    int b = 36;
    printf("%lld\n", tribonacci(a));
    printf("%lld\n", tribonacci(b));
    return 0;
}

