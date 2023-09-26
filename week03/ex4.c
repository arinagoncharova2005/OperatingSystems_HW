#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
void* add_int(const void* a, const void* b) {
    int* res = malloc(sizeof(int));
    *res = *(const int*)a + *(const int*)b;
    return (void*)res;
}
void* add_double(const void* a, const void* b) {
    double* res = malloc(sizeof(double));
    *res = *(const double*) a + *(const double*) b;
    return (void*)res;
}
void* multiply_int(const void* a, const void* b) {
    int* res = malloc(sizeof(int));
    *res = *(const int*) a * *(const int*) b;
    return (void *)res;
}
void* multiply_double(const void* a, const void* b) {
    double* res = malloc(sizeof(double));
    *res = *(const double*) a * *(const double*) b;
    return (void*)res;
}
void* max_int(const void* a, const void* b) {
    int* res = malloc(sizeof(int));
    if (*(const int*)a > *(const int*)b){
        *res = *(const int*)a;
    } else {
        *res = *(const int *) b;
    }
    return (void *)res;
}
void* max_double(const void* a, const void* b) {
    double* res = malloc(sizeof(double));
    if (*(const double*)a > *(const double*)b) {
        *res = *(const double*) a;
    } else {
        *res = *(const double *) b;
    }
    return (void*)res;
}
void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    if (size == sizeof(int)) {
        //int* elem = (int*) base;
        int * res = malloc(sizeof(int));
        *res = *(int *) initial_value;
        int* elem = (int *) base;
        for (int i = 0; i < n; i++) {
            int* temp = (int*) opr((void *)res, elem + i);
            *res = *temp;
            free(temp);
        }
        return (void *)res;
    } else {
        double* res = malloc(sizeof(double));
        *res = *(double*) initial_value;
        double* elem = (double*) base;
        for(int i = 0; i<n; i++) {
            double* temp = (double*) opr((void*)res, elem+i);
            *res = *temp;
            free(temp);
        }
        return (void*)res;
    }
}

int main() {
    //FOR INTEGERS
    int arr_i[5] = {4, 6, 1, -100, 56};
    printf("Array of integer numbers: \n");
    for (int i = 0; i<sizeof(arr_i)/sizeof(int); i++) {
        printf("%d ", arr_i[i]);
    }
    printf("\n");

    //sum of 5 integers
    int initial_sum_value_int = 0;
    void* initial_sum_value_int_p = (void*)&initial_sum_value_int;

    void* sum_int = aggregate(arr_i, sizeof(int), 5, initial_sum_value_int_p, add_int);
    printf("The sum of integers is %d\n", *(int*)sum_int);
    free(sum_int);

    //product of 5 integers
    int initial_product_value_int = 1;
    void* initial_product_value_int_p = (void*)&initial_product_value_int;
    void* product_int = aggregate(arr_i, sizeof(int), 5, initial_product_value_int_p, multiply_int);
    printf("The product of integers is %d\n", *(int*)product_int);
    free(product_int);

    //max in array of integers
    int minimum_value_int = INT_MIN;
    int* minimum_value_int_p = &minimum_value_int;
    void* maximum_int_res = aggregate(arr_i, sizeof(int), 5, minimum_value_int_p, max_int);
    printf("The maximum value of integer in the array is %d\n", *(int*)maximum_int_res);
    free(maximum_int_res);

    //FOR DOUBLES
    double arr_d[5] = {2.5, 6, 9.1, -1.5, 4.6};
    printf("Array of double numbers: \n");
    for (int i = 0; i<sizeof(arr_d)/sizeof(double); i++) {
        printf("%f ", arr_d[i]);
    }
    printf("\n");
    //sum of 5 doubles
    double initial_sum_value_double = 0.0;
    void* initial_sum_value_double_p = (void*)&initial_sum_value_double;
    void* sum_double = aggregate(arr_d, sizeof(double), 5, initial_sum_value_double_p, add_double);
    printf("The sum of doubles is %f\n", *(double*)sum_double);
    free(sum_double);
    //product of 5 doubles
    double initial_product_value_double = 1.0;
    void* initial_product_value_double_p = (void*)&initial_product_value_double;
    void* product_double = aggregate(arr_d, sizeof(double), 5, initial_product_value_double_p, multiply_double);
    printf("The product of doubles if %f\n", *(double*)product_double);
    free(product_double);
    //max in array of doubles
    double initial_max_double = -DBL_MAX;
    void* initial_max_double_p = (void*)&initial_max_double;
    void* max_double_res = aggregate(arr_d, sizeof(double), 5, initial_max_double_p, max_double);
    printf("The maximum value in the double array is %f\n", *(double *)max_double_res);
    free(max_double_res);

    return 0;
}
