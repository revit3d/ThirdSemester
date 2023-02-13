#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int
main (int argc, char *argv[]) {
    enum { 
        NSIZE = sizeof(long long),
        INT64_MIN = (long long)(1ull << 63)
    };
    int file = 0;
    if (argc == 0 || (file = open(argv[1], O_RDWR)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    long long rd;
    long long min;
    int pos_min = -1;
    int cur = 0;
    int first = 1;
    while (read(file, &rd, NSIZE) == NSIZE) {
        if (first) {
            first = 0;
            min = rd;
            pos_min = cur;
        } else if (rd < min) {
            min = rd;
            pos_min = cur;
        }
        cur++;
    }
    if (pos_min == -1) {
        return 0;
    }
    if (min != INT64_MIN) {
        min = -min;
    }
    lseek(file, pos_min * NSIZE, SEEK_SET);
    if (write(file, &min, NSIZE) != NSIZE) {
        fprintf(stderr, "Cannot write to file\n");
        return 1;
    }
    close(file);

    return 0;
}