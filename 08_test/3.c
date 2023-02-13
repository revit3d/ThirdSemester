#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>

int
main (int argc, char *argv[]) {
    int size;
    int len;
    int fd = open(argv[1], O_RDWR);
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    len = sqrt(size * 8.0);
    char buf;
    int col = 0, row = 0;
    for (int i = 0; i < size; i++) {
        read(fd, &buf, 1);
        for (int j = 0; j < 8; j++) {
            if (row == col) {
                buf |= (1 << j);
            }
            //printf("%d %d\n", col, row);
            //printf("%d\n", buf);
            row++;
            if (row == len) {
                col++;
                row = 0;
            }
        }
        lseek(fd, -1, SEEK_CUR);
        write(fd, &buf, 1);
    }
    close(fd);

    return 0;
}