#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int
main (int argc, char *argv[]) {
    enum { 
        BASE = 10,
        THOUSAND = 1000,
    };
    long long res = 0;
    long long num;
    char *end;
    for (int i = 1; i < argc; i++) {
        end = NULL;
        errno = 0;
        num = strtol(argv[i], &end, BASE);
        if (errno || end == argv[i] || (int)num != num) {
            continue;
        }
        if (*end == 'k') {
            num *= THOUSAND;
            end++;
        }
        if (*(end + 1) != '\0') {
            continue;
        }
        if (*end == '+') {
            res += num;
        } else if (*end == '-') {
            res -= num;
        }
    }
    printf("%lld\n", res);

    return 0;
}