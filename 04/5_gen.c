#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[]) {
    enum { NSIZE = sizeof(long long) };
    int file = 0;
    if ((file = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0600)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    long long buf = atoi(argv[2]);
    if (write(file, &buf, NSIZE) != NSIZE) {
        fprintf(stderr, "Cannot write to file\n");
        return 1;
    }
    close(file);

    return 0;
}