#include <stdio.h>

double pow2(int a, double b) {
    printf("%d %lf\n", a, b);
    double res = 1;
    while (b > 0) {
        res *= a;
        b--;
    }
    return res;
}

char *sendfuck(char *msg) {
    return msg;
}