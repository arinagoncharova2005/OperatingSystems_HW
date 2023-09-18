#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
    int a = INT_MAX;
    unsigned short int b = USHRT_MAX;
    signed long int c =  LONG_MAX;
    float d = FLT_MAX;
    double e = DBL_MAX;
    printf("Int value: %d, int size: %zu bytes\n", a, sizeof(a));
    printf("Unsigned short int value: %hu, unsigned short int size: %zu bytes\n", b, sizeof(b));
    printf("Signed long int: %ld, signed long size: %zu bytes\n", c, sizeof(c));
    printf("Float value: %f, float size: %zu bytes\n", d, sizeof(d));
    printf("Double value: %f, double size: %zu bytes\n", e, sizeof(e));
    return 0;
}

