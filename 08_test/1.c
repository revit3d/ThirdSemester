#include <stdio.h>
#include <stdlib.h>

int 
comp(const long long *left, const long long *right) {
    return *left < *right;
}

int
main (void) {
    long long sum = 0;
    long long input[100000];
    int size;
    for (size = 0; scanf("%lld", &input[size]) == 1; size++) {
        continue;
    }
    qsort(input, size, sizeof(long long), (int(*) (const void *, const void *)) comp);
    int i, j;
    for (i = 0, j = size - 1; i < j; i++, j--) {
        sum += input[i] + input[j];
    }
    if (i == j) {
        sum += input[i];
    }
    printf("%lld\n", sum);

    return 0;
}