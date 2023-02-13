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
            switch (state) {
            case OK:
                printf("%lld\n", num);
                state = SKIP;
                num = 0;
                break;
            case ERROR:
                state = SKIP;
                num = 0;
                break;
            case SKIP:
                break;
            }
            continue;
        }
        if (state == SKIP) {
            state = OK;
        } else if (state == ERROR) {
            continue;
        }
        switch (dgt) {
        case '1':
            if (num == LLONG_MIN / 3 - 1) {
                num = LLONG_MIN;
            } else  if (__builtin_smulll_overflow(num, 3ll, &num)
                || __builtin_saddll_overflow(num, 1ll, &num)) {
                state = ERROR;
                printf("%s\n", err);
            }
            break;
        case '0':
            if (__builtin_smulll_overflow(num, 3ll, &num)) {
                state = ERROR;
                printf("%s\n", err);
            }
            break;
        case 'a':
            if (__builtin_smulll_overflow(num, 3ll, &num)
                || __builtin_ssubll_overflow(num, 1ll, &num)) {
                state = ERROR;
                printf("%s\n", err);
            }
            break;
        }
    }
    if (state == OK) {
        printf("%lld\n", num);
    }

    return 0;
}