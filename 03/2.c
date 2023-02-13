#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int 
main (int argc, char *argv[]){
    long long sum_pos = 0;
    long long sum_neg = 0;
    errno = 0;
    for (int arg = 1; arg < argc; arg++) {
        char *end;
        const int cur_num = strtol(argv[arg], &end, 10);
        if (errno) {
            fprintf(stderr, "NaN among passed arguments\n");
            exit(1);
        }
        if (cur_num >= 0) {
            if (__builtin_add_overflow(sum_pos, cur_num, &sum_pos)) {
                fprintf(stderr, "integer overflow\n");
                exit(1);
            }
        } else if (cur_num < 0) {
            if (__builtin_add_overflow(sum_neg, cur_num, &sum_neg)) {
                fprintf(stderr, "integer overflow\n");
                exit(1);
            }
        }
    }
    printf("%lld\n%lld\n", sum_pos, sum_neg);

    return 0;
}