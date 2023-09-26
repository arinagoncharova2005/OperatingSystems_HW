#include <stdio.h>
#include <stdlib.h>

int const_tri(int *p, int *n) {
    if (*n==0) {
        return 0;
    }
    (*n)--;
    for (int i = 3; i <= *n; i++) {
        *(p + (i % 3)) = *p + *(p + 1) + *(p + 2);
    }
    return *(p + (*n % 3));

}

int main() {
    const int x = 1;
    const int *q = &x;
    int *const p = malloc(3 * sizeof(int));
    *p = x;
    *(p + 1) = *q;
    *(p + 2) = 2 * x;
    printf("Memory address of the first cell: %p \n", p);
    printf("Memory address of the first cell: %p \n", p + 1);
    printf("Memory address of the first cell: %p \n", p + 2);
    printf("The size of int is %zu\n", sizeof(int));

    //Checking that the cells are contiguous
    if ((void*)p+sizeof(int) == p+1) {
        printf("The cells are contiguous\n");
    } else {
        printf("The cells are not contiguous\n");
    }

    int n = 15;
    printf("The tribonacci result for %d is %d\n", n, const_tri(p, &n));
    free(p);
    return 0;
}
