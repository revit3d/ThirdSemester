#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    enum {
        MEMSIZE_ARG = 1,
        CACHESIZE_ARG = 2,
        BLCKSIZE_ARG = 3,
        HEXADECIMAL_BASE = 16,
        DECIMAL_BASE = 10,
        OP_SIZE = 5,
    };
    char op[OP_SIZE] = {};
    unsigned size, value;
    unsigned ans = 0;
    int *cache;
    unsigned long memory_size = atol(argv[MEMSIZE_ARG]);
    unsigned long cache_size = atol(argv[CACHESIZE_ARG]);
    unsigned long block_size = atol(argv[BLCKSIZE_ARG]);
    unsigned long adr;
    memory_size /= block_size;
    cache_size /= block_size;
    cache = calloc(cache_size, sizeof(int));
    for (int i = 0; i < cache_size; i++) {
        cache[i] = -1;
    }
    while (scanf("%s %lx %u %u", op, &adr, &size, &value) > 0) {
        if (op[0] != 'R') {
            continue;
        }
        adr /= block_size;
        if (cache[adr % cache_size] != adr && cache[adr % cache_size] != -1) {
            ans++;
        }
        cache[adr % cache_size] = adr;
    }
    printf("%u\n", ans);
    free(cache);

    return 0;
}