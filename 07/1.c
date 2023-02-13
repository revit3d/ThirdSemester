#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

int
main (void) {
    enum {
        OK = 0,
        ERROR = -1,
        SKIP = 1,
        BASE = 3,
    };
    const char *err = "18446744073709551616";
    long long num = 0;
    int state = SKIP;
    int dgt;
    while ((dgt = getchar()) != EOF) {
        if (dgt != 'a' && dgt != '0' && dgt != '1') {
            if (state == OK) {
                printf("%lld\n", num);
            }
            state = SKIP;
            num = 0;
            continue;
        }
        if (state == SKIP) {
            state = OK;
        } else if (state == ERROR) {
            continue;
        }
        if (dgt == '1' && num == LLONG_MIN / BASE - 1) {
            num = LLONG_MIN;
        } else if (__builtin_smulll_overflow(num, BASE, &num)) {
            state = ERROR;
            printf("%s\n", err);
        } else if (dgt == '1' && __builtin_saddll_overflow(num, 1ll, &num)) {
            state = ERROR;
            printf("%s\n", err);
        } else if (dgt == 'a' && __builtin_ssubll_overflow(num, 1ll, &num)) {
            state = ERROR;
            printf("%s\n", err);
        }
    }
    if (state == OK) {
        printf("%lld\n", num);
    }

    return 0;
}