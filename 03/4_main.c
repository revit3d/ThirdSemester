#include <stdio.h>

extern int satsum(int v1, int v2);

int
main (void) {
    int v1, v2;
    scanf("%d %d", &v1, &v2);
    printf("%d\n", satsum(v1, v2));

    return 0;
}