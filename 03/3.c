#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int 
main (int argc, char *argv[]) {
    double value = 0;
    if (sscanf(argv[1], "%lf", &value) != 1) {
        fprintf(stderr, "Too little arguments\n");
        exit(1);
    }
    for (int arg = 2; arg < argc; arg++) {
        double cur_num;
        if(sscanf(argv[arg], "%lf", &cur_num) != 1) {
            fprintf(stderr, "NaN among passed arguments\n");
            exit(1);
        }
        value += value / 100 * cur_num;
        value = round(value * 10000) / 10000;
    }
    printf("%.04lf\n", value);

    return 0;
}