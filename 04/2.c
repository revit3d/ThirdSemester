#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[]) {
    enum { NSIZE = sizeof(double) };
    int file = 0;
    double prev, cur;
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }
    if ((file = open(argv[1], O_RDWR)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    read(file, &prev, NSIZE);
    int N = atoi(argv[2]);
    for (int i = 1; i < N; i++) {
        if (read(file, &cur, NSIZE) != NSIZE) {
            break;
        }
        cur = cur - prev;
        lseek(file, -8, SEEK_CUR);
        if (write(file, &cur, NSIZE) != NSIZE) {
            fprintf(stderr, "Cannot write to file\n");
            return 1;
        }
        prev = cur;
    }
    close(file);

    return 0;
}