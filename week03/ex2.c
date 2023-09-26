#include <stdio.h>
#include <math.h>

struct Point {
    double x;
    double y;
};

double distance(struct Point p1, struct Point p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

double area(struct Point p1, struct Point p2, struct Point p3) {
    return 0.5 * fabs(p1.x*p2.y - p2.x*p1.y + p2.x*p3.y - p3.x*p2.y + p3.x*p1.y - p1.x*p3.y);
}

int main() {
    struct Point A;
    A.x = 2.5;
    A.y = 6;
    struct Point B;
    B.x = 1;
    B.y = 2.2;
    struct Point C;
    C.x = 10;
    C.y = 6;

    printf("Distance between points A and B %f\n", distance(A, B));
    printf("Area of ABC %f\n", area(A, B, C));
    return 0;
}
