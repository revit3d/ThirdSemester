#include <stdio.h>

int
gcd (int a, int b, int *x, int *y){
    if (a == 0){
        *x = 0; 
        *y = 1;
        return b;
    }
    int x1, y1;
    int d = gcd (b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return d;
}

int
main(void){
    enum { MAXN = 2000 + 1 };
    int n;
    int coef[MAXN];
    if(scanf("%d", &n) != 1){
        return 1;
    }
    if(n <= 1 || n >= MAXN)
        return 1;
    for(int a = 1; a < n; a++){
        if(gcd(a, n, &coef[a], &coef[MAXN - 1]) != 1)
            return 1;
        printf("0 ");
    }
    printf("\n");
    for(int c = 1; c < n; c++){
        for(int a = 1; a < n; a++)
            printf("%d ", ((coef[a] * c) % n + n) % n);
        printf("\n");
    }

    return 0;
}