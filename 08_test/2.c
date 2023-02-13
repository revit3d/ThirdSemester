#include <stdio.h>
#include <stdlib.h>

int comp (const unsigned long long *left, const unsigned long long *right) {
    return *right < *left;
}

int
main (int argc, char *argv[]) {
    unsigned long long input[100000];
    char *endptr;
    int size;
    for (size = 0; size + 1 < argc; size++) {
        input[size] = strtoull(argv[size + 1], &endptr, 17);
    }
    qsort(input, size, sizeof(unsigned long long), (int(*) (const void *, const void *)) comp);
    for (int i = size - 1; i >= 0; i--) {
        printf("%llu\n", input[i]);
    }

    return 0;
}