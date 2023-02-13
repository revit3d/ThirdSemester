#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum { NODESIZE = sizeof(int) * 3 };

void dfs(int file) {
    int buf[3];
    if (read(file, buf, NODESIZE) != NODESIZE) {
        fprintf(stderr, "Cannot read from file\n");
        exit(1);
    }
    if (buf[2] != 0) {
        lseek(file, buf[2] * NODESIZE, SEEK_SET);
        dfs(file);
    }
    printf("%d\n", buf[0]);
    if (buf[1] != 0) {
        lseek(file, buf[1] * NODESIZE, SEEK_SET);
        dfs(file);
    }
}

int
main (int argc, char *argv[]) {
    int file = 0;
    if ((file = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "Cannot open file\n");
        return 1;
    }
    dfs(file);
    close(file);

    return 0;
}